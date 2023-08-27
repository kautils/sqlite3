#include "../include/kautil/sqlite3/sqlite3.h"
#include <cassert>

#include "sqlite3.h"
#include <filesystem>
#include <fstream>





namespace kautil{
namespace database {
int kSQLITE_DETERMINISTIC = SQLITE_DETERMINISTIC;
int kSQLITE_DIRECTONLY = SQLITE_DIRECTONLY;
int kSQLITE_SUBTYPE = SQLITE_SUBTYPE;
int kSQLITE_INNOCUOUS = SQLITE_INNOCUOUS;

int  kSQLITE_UTF8 =SQLITE_UTF8;
int  kSQLITE_UTF16LE=SQLITE_UTF16LE;
int  kSQLITE_UTF16BE=SQLITE_UTF16BE;
int  kSQLITE_UTF16=SQLITE_UTF16;
int  kSQLITE_ANY=SQLITE_ANY;
int  kSQLITE_UTF16_ALIGNED=SQLITE_UTF16_ALIGNED;


struct Sqlite3Internal{
    
    Sqlite3Internal(){};
    ~Sqlite3Internal(){
        
        auto arr = reinterpret_cast<sqlite3_stmt**>(stmt_releaser.data());
        auto arr_len = stmt_releaser.size()/sizeof(uintptr_t);
        for(auto i = 0; i < arr_len; ++i){
            if(arr[i])sqlite3_finalize(arr[i]); 
        }
        stmt_releaser.clear();
        if (auto_close_ && database_){
            sqlite3_close(database_);
        } 
        if(last_serialize_buffer) free(last_serialize_buffer);
        
    };
    
    sqlite3 *database_ = nullptr;
    bool verbose_ = true;
    bool auto_close_ = true;
    std::string scheme="main"; // main, temp. i don't know this well yet.
    unsigned char * last_serialize_buffer =nullptr;
    std::string stmt_releaser;
};



//sqlite3_destructor_type Sqlite3::kSQLITE_STATIC = SQLITE_STATIC;
//sqlite3_destructor_type Sqlite3::kSQLITE_TRANSIENT = SQLITE_TRANSIENT;

const char *Sqlite3::scheme() { return m->scheme.data(); }
const char *Sqlite3::FileName() {
    return sqlite3_db_filename((sqlite3*)database(), m->scheme.data());
}




int DefaultCallBack(void *custom_parameter, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s( %s ), ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


bool Sqlite3::error_msg(){
    fprintf(stderr,"%s",sqlite3_errmsg(m->database_));
    fflush(stderr);
    return true;
}

const char * Sqlite3::error_string(){
    return sqlite3_errmsg(m->database_);
}


#define __error_print printf("%s(%d) %s\n",__FILE__,__LINE__,error_string()); 

Sqlite3::Sqlite3(const char dbpath[],int flags/*, bool*/ ) : m(new Sqlite3Internal()){
    if(sqlite3_open_v2(
            (char *) dbpath
            ,&m->database_
            ,flags
            , nullptr) != SQLITE_OK ){
        __error_print;exit(1);
    }
}


Sqlite3::Sqlite3(const char dbpath[]) : m(new Sqlite3Internal()) {
    if (sqlite3_open((char *) dbpath, &m->database_)) {
        __error_print;exit(1);
    }
};


Sqlite3::Sqlite3(const void *ser_data, const uint64_t &ser_data_bytes, const char *scheme,int openflag) : m(new Sqlite3Internal()) {
    if(sqlite3_open_v2(":memory:", &m->database_, openflag, nullptr) == SQLITE_OK ){
        sqlite3_deserialize(m->database_
                            , scheme
                            , reinterpret_cast<unsigned char*>(const_cast<void*>(ser_data))
                            , static_cast<int64_t>(ser_data_bytes)
                            , static_cast<int64_t>(ser_data_bytes)
                            , SQLITE_DESERIALIZE_READONLY);
    }else{
        __error_print;exit(1);
    }
}




Sqlite3::Sqlite3(const void *buffer_including_db, const uint64_t &dbsize_exact, const uint64_t &buffer_including_db_bytes, const char *scheme,int openflag) : m(new Sqlite3Internal()){
    if(sqlite3_open_v2(":memory:", &m->database_, openflag, nullptr) == SQLITE_OK ){
        sqlite3_deserialize(m->database_
                            , scheme
                            , reinterpret_cast<unsigned char*>(const_cast<void*>(buffer_including_db))
                            , static_cast<int64_t>(dbsize_exact)
                            , static_cast<int64_t>(buffer_including_db_bytes)
                            , SQLITE_DESERIALIZE_RESIZEABLE);
    }else{
        __error_print;exit(1);
    }
}

Sqlite3::Sqlite3(sqlite3 *database) : m(new Sqlite3Internal()){
    m->database_ = database;
    m->auto_close_ = false;
}


int Sqlite3::vacuum() { return vacuum(nullptr); }
int Sqlite3::vacuum(const char *file) {
    SendQuery("PRAGMA auto_vacuum = FULL;");
    auto vaccume= file ? Compile(("vacuum " + m->scheme + " into '" + std::string{file} + "'").data()) : Compile(("vacuum " + m->scheme).data()); 
    auto res= Step(&vaccume,false);
    SendQuery("PRAGMA auto_vacuum = NONE;");
    return res;
}


int Sqlite3::CreateFunction(
    const char *name
    , int nArg
    , int eTextRep
    , const void * opaque
    , Sqlite3::sql_function_type fp
    , Sqlite3::sql_step_function_type step
    , Sqlite3::sql_final_function_type final
    , Sqlite3::sql_destroy_function_type destroy) {
    sqlite3_create_function_v2(m->database_, name, nArg, eTextRep, const_cast<void*>(opaque), fp, step, final, destroy);
    return 0;
}


Sqlite3::~Sqlite3() {
    delete m;
};

sqlite3 * Sqlite3::database() { return m->database_; }

// serialize with nocopy
uint64_t Sqlite3::serialize(void ** res){
    int64_t res_bytes=0;
    if(m->last_serialize_buffer) free(m->last_serialize_buffer);
    m->last_serialize_buffer = sqlite3_serialize(m->database_, m->scheme.data(), &res_bytes, 0); // NOCOPY dosen't work
    if(res)*res=m->last_serialize_buffer;
    return *res ? static_cast<uint64_t>(res_bytes) : 0;
}

uint64_t Sqlite3::serialize_detatch(void ** res){
    if(res){
        int64_t res_bytes=0;
        *res =(void*) sqlite3_serialize(m->database_, m->scheme.data(), &res_bytes, 0); // NOCOPY dosen't work
        return static_cast<uint64_t>(res_bytes);
    }else{
        return 0;
    }
}


Sqlite3Stmt Sqlite3::RowidMaxStmt(const char *table_name, const char *col_name_of_id) {
    return Compile((std::string{"select "} + col_name_of_id  + " from " + table_name + " order by " + col_name_of_id  + " desc limit 1").c_str());
}

Sqlite3Stmt Sqlite3::RowidMinStmt(const char *table_name, const char *col_name_of_id) {
    return Compile((std::string{"select "} + col_name_of_id  + " from " + table_name + " order by " + col_name_of_id  + " asc limit 1").c_str());
}

uint64_t Sqlite3::RowidMax(const char *table_name,const char col_name_of_id[]) {
    auto stmt_getrowid_s =Compile((std::string{"select "} + col_name_of_id  + " from " + table_name + " order by " + col_name_of_id  + " desc limit 1").c_str());
    if(Step(&stmt_getrowid_s,false) < SQLITE_ROW) return -1;
    return static_cast<uint64_t>(sqlite3_column_int64(stmt_getrowid_s.stmt(),0));
}

uint64_t Sqlite3::RowidMin(const char *table_name,const char col_name_of_id[]) {
    auto stmt_getrowid_s =Compile((std::string{"select "} + col_name_of_id  + " from " + table_name + " order by " + col_name_of_id  + " asc limit 1").c_str());
    if(Step(&stmt_getrowid_s,false) < SQLITE_ROW) return -1;
    return static_cast<uint64_t>(sqlite3_column_int64(stmt_getrowid_s.stmt(),0));
}

void Sqlite3::SetVerbose(bool v) { m->verbose_=v; }
bool Sqlite3::SendQuery(const char query[],const void *parameter, decltype(DefaultCallBack) fp_callback) {
    char *tail;
    auto rc = sqlite3_exec(m->database_, query, fp_callback, const_cast<void*>(parameter), &tail);
    if (rc != SQLITE_OK) {
        if(m->verbose_){
            fprintf(stderr,"%s\n",tail);
            fflush(stderr);
            sqlite3_free(tail);
        }
        return false;
    } else {
        return true;
    }

};


bool
Sqlite3::SendQueryWithFile(const char path[],const void *parameter, decltype(DefaultCallBack) fp_callback) {
    using std::chrono::system_clock;
    auto st = system_clock::now();
    namespace fs = std::filesystem;
    std::string query;
    auto file = fs::path(path);
    query.resize(fs::file_size(file));
    std::ifstream(file, std::ios::binary | std::ios::in).read(query.data(), query.size());
    SendQuery(query.data(), parameter, fp_callback);

    printf("%s\n","<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    printf("%s\n",query.data());
    printf("%f(sec)\n\n",(double) (system_clock::now() - st).count() / system_clock::period::den);
    printf("%s\n",">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    return true;

}


struct Sqlite3StmtInternal{
    std::string * entity = nullptr;
    uint64_t pos=0;
};



sqlite3_stmt * Sqlite3Stmt::stmt(){
    return m->entity ? *reinterpret_cast<sqlite3_stmt**>(m->entity->data()+m->pos) : nullptr;
}

Sqlite3Stmt::Sqlite3Stmt(Sqlite3Stmt && v){
    delete m;
    m = v.m;
    v.m = nullptr;
}

Sqlite3Stmt & Sqlite3Stmt::operator=(Sqlite3Stmt && v){
    delete m;
    m = v.m;
    v.m= nullptr;
    return *this;
}


void Sqlite3Stmt::release(){
    if(m->entity){
        if(auto ptr = stmt()){
            //printf("--- %llx\n",ptr); fflush(stdout);
            sqlite3_finalize(ptr);
            static auto kNl = nullptr;
            memcpy(m->entity->data()+m->pos,&kNl,sizeof(uintptr_t));
            
        }
    }
}



Sqlite3Stmt::Sqlite3Stmt() : m(new Sqlite3StmtInternal){  }
Sqlite3Stmt::~Sqlite3Stmt() {
    delete m;
    // forbit implicit release
    // instead  permit explicit release
    // release is done by Sqlite3 object which generate this stmt.
}



Sqlite3Stmt Sqlite3::Compile(const char query[], int nByte, const char **pzTail) { 
    return std::move(Compile(query, nullptr,nByte,pzTail));
}
Sqlite3Stmt Sqlite3::Compile(const char query[],bool * error, int nByte, const char **pzTail) {
    auto stmt = (sqlite3_stmt *) nullptr;
    if (sqlite3_prepare_v2(m->database_, query, nByte, &stmt, pzTail) != SQLITE_OK){
        if(m->verbose_){
            fprintf(stderr,"prepare failed: %s\n",error_string());
            fflush(stderr);
        }
        if(error) *error =true;
        return Sqlite3Stmt{};
    }else {
        Sqlite3Stmt res{};
        if(error) *error =false;
        {
            // if sqlite3 is closed before finalize res.ptr, memory leak will occure. 
            // i want to confirm to release stmt Compiled here before database close.  

            res.m->entity = &m->stmt_releaser;
            res.m->pos = m->stmt_releaser.size();
            res.m->entity->resize(sizeof(uintptr_t)+res.m->entity->size());
            memcpy(res.m->entity->data()+res.m->pos,&stmt,sizeof(uintptr_t));

            //printf("+++ %llx %llx %lld\n",(uintptr_t) res.stmt(),(uintptr_t)stmt,res.m->pos);
            //fflush(stdout);


        }
        /*res.ptr=*/

        return std::move(res);
    }
}



Sqlite3Stmt Sqlite3::CompileAllowAsterisk(const char query[], int nByte, const char **pzTail) {
    auto org = std::string_view{query};
    auto pos = org.find("*");
    if(pos == std::string::npos) return Compile(query, nByte, pzTail);
    static  auto kFromAtStr = std::string{" from "};
    auto sv_query = std::string_view{query};
    auto from_at = sv_query.find(" from ");
    if(from_at == std::string::npos) return Compile(query, nByte, pzTail);

    auto table_name = std::string{sv_query.substr(from_at + kFromAtStr.size())};{
        auto cnt = int(0);
        for(; cnt < table_name.size(); ++cnt)  if(table_name[cnt] == '(' || table_name[cnt] == ' ') break;
        table_name.resize(cnt);
    }

    auto cols = std::string{};{
        auto const q_col = std::string{"select * from "}.append(table_name).append(" limit 1");
        auto stmt = Compile(q_col.c_str());
        auto maxi = sqlite3_column_count(stmt.stmt());
        for(auto i = 0; i < maxi; ++i){
            cols.append(sqlite3_column_name(stmt.stmt(),i)).push_back(',');
        }
        if(!cols.empty()) cols.resize(cols.size() - 1);
    }



    auto asterisk = std::string{org.substr(0,pos)} + cols + std::string{org.substr(pos+1,-1)};
    for(;;){
        if( (pos = asterisk.find("*",pos+cols.size())) == std::string::npos) break;
        asterisk = asterisk.substr(0,pos) + cols + asterisk.substr(pos+1,-1);
    }

    return Compile(asterisk.data(),nByte,pzTail);
}

int Sqlite3::Step(Sqlite3Stmt *stmt , bool reset ) {
    auto res =  sqlite3_step(stmt->stmt());
    if(reset) sqlite3_reset(stmt->stmt());
    return res;
}



bool Sqlite3::Reset(Sqlite3Stmt *stmt){
    return sqlite3_reset(stmt->stmt()) == SQLITE_OK;
}

int Sqlite3::StepAll(Sqlite3Stmt *stmt) {
    auto res =-1;
    try{
        for(;res = sqlite3_step(stmt->stmt());){
            if(res == SQLITE_DONE) break;
            if(res != SQLITE_ROW) return res;
        }
    }catch(...){
        error_msg();
        return -1;
    }
    sqlite3_reset(stmt->stmt());
    return res;
}



/**
 @note : refer from sqlite3.h
    SQLITE_API int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
    SQLITE_API int sqlite3_bind_blob64(sqlite3_stmt*, int, const void*, sqlite3_uint64, void(*)(void*));
    SQLITE_API int sqlite3_bind_double(sqlite3_stmt*, int, double);
    SQLITE_API int sqlite3_bind_int(sqlite3_stmt*, int, int);
    SQLITE_API int sqlite3_bind_int64(sqlite3_stmt*, int, sqlite3_int64);
    SQLITE_API int sqlite3_bind_null(sqlite3_stmt*, int);
    SQLITE_API int sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*));
    SQLITE_API int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int, void(*)(void*));
    SQLITE_API int sqlite3_bind_text64(sqlite3_stmt*, int, const char*, sqlite3_uint64, void(*)(void*), unsigned char encoding);
    SQLITE_API int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
    SQLITE_API int sqlite3_bind_pointer(sqlite3_stmt*, int, void*, const char*,void(*)(void*));
    SQLITE_API int sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);
    SQLITE_API int sqlite3_bind_zeroblob64(sqlite3_stmt*, int, sqlite3_uint64);
*/




bool Sqlite3::SetInt(Sqlite3Stmt * stmt, int const& i , int const& v ){ return sqlite3_bind_int(stmt->stmt(), i, v) == SQLITE_OK; }
bool Sqlite3::SetDouble(Sqlite3Stmt* stmt, const int &i, const double &v) { return sqlite3_bind_double(stmt->stmt(), i, (double) v) == SQLITE_OK; }
bool Sqlite3::SetInt64(Sqlite3Stmt* stmt, int const& i , uint64_t const& v ){ return sqlite3_bind_int64(stmt->stmt(), i, static_cast<int>(v)) == SQLITE_OK; }
bool Sqlite3::SetText(Sqlite3Stmt* stmt, int const& i , const char * data, uint64_t const& size ,sqlite3_life lifetime ){ return sqlite3_bind_text(stmt->stmt(), i, data, static_cast<int>(size), (lifetime == life_static) ? SQLITE_STATIC : SQLITE_TRANSIENT ) == SQLITE_OK;  }
bool Sqlite3::SetBlob(Sqlite3Stmt* stmt, const int &i,const void * data, uint64_t size, sqlite3_life lifetime) {
    return sqlite3_bind_blob(stmt->stmt(), i, data, static_cast<int>(size), (lifetime == life_static) ? SQLITE_STATIC : SQLITE_TRANSIENT ) == SQLITE_OK;
}
bool Sqlite3::SetNull(Sqlite3Stmt* stmt, const int &i,const void * data, uint64_t size, sqlite3_life lifetime) { return sqlite3_bind_null(stmt->stmt(), static_cast<int>(i)) == SQLITE_OK; }
bool Sqlite3::SetZeroBlob(Sqlite3Stmt* stmt, const int &i, int n){ return sqlite3_bind_zeroblob(stmt->stmt(),i,n) == SQLITE_OK; }

constexpr char *kQueryBeginTransaction = (char *) "begin transaction;";
constexpr char *kQueryCommit = (char *) "commit;";
constexpr char *kQueryRollback = (char *) "rollback;";

bool Sqlite3::BeginTransaction() { return SendQuery(kQueryBeginTransaction); }

bool Sqlite3::EndTransaction() { return SendQuery(kQueryCommit); }

bool Sqlite3::Commit() { return SendQuery(kQueryCommit); }
bool Sqlite3::Rollback() { return SendQuery(kQueryRollback); }




} //namespace database {
} //namespace kautil{


