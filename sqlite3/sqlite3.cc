#include "../sqlite3/sqlite3.h"
#include <sqlite3.h>
#include <string>

static_assert(sizeof(sqlite3_int64) == sizeof(rowid_t));

#include "c11_string_allocator/c11_string_allocator.h"


namespace kautil{
namespace database {


struct sqlite3_exception : std::exception{
    std::string msg;
    sqlite3_exception(int size,const char * fmt,...){
        msg.resize(size);
        va_list l;
        va_start(l,fmt);
        __mingw_vsprintf(msg.data(),fmt,l);
        va_end(l);
    }
    const char * what() const noexcept override{ return msg.data(); }
};


int send_query_default_call_back(void *NotUsed, int argc, char **argv, char **azColName){ return 0; }
void release_stmt(Sqlite3Stmt * stmt){ if(stmt) stmt->release(); }


struct Sqlite3Internal{
    Sqlite3Internal(){};
    ~Sqlite3Internal(){
        if (auto_close_ && con){
            sqlite3_close(con);
        } 
        release_stmt(begin_transaction);
        release_stmt(end_transaction);
        release_stmt(roll_back);
        release_stmt(row_max);
        release_stmt(row_min);
        release_stmt(foreign_key_sw);
        release_stmt(vcm);
        release_stmt(vcm_off);
        release_stmt(vcm_on);
    };
    
    ::sqlite3 *con = nullptr;
    bool auto_close_ = true;
    std::string schema="main"; // main, temp. i don't know this well yet.
    Sqlite3Stmt * begin_transaction = 0;
    Sqlite3Stmt * end_transaction = 0;
    Sqlite3Stmt * roll_back=0;
    Sqlite3Stmt * row_max=0;
    Sqlite3Stmt * row_min=0;
    Sqlite3Stmt * foreign_key_sw=0;
    Sqlite3Stmt * vcm_on=0;
    Sqlite3Stmt * vcm_off=0;
    Sqlite3Stmt * vcm=0;
};



const char *Sqlite3::schema() { return m->schema.data(); }
const char *Sqlite3::file_name() { return sqlite3_db_filename((::sqlite3*)connection(), m->schema.data()); }

[[nodiscard]] sqlite3::Blob* Sqlite3::blob(const char schema[], const char table[],const char colname[]){
    return kautil::database::sqlite3::sqlite3_blob(m->con, schema, table, colname);
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
    fprintf(stderr,"%s",sqlite3_errmsg(m->con));
    fflush(stderr);
    return true;
}

const char * Sqlite3::error_string(){
    return sqlite3_errmsg(m->con);
}


#define __error_print printf("%s(%d) %s\n",__FILE__,__LINE__,error_string()); 

kautil::database::Sqlite3::Sqlite3(): m(new Sqlite3Internal) {}
Sqlite3::Sqlite3(const char dbpath[],int flags ) : m(new Sqlite3Internal()){ setup(dbpath,flags); }
Sqlite3::Sqlite3(const char dbpath[]) : m(new Sqlite3Internal()) { setup(dbpath); };
Sqlite3::Sqlite3(const void *ser_data, const uint64_t &ser_data_bytes, const char *schema,int openflag) : m(new Sqlite3Internal()) { setup(ser_data,ser_data_bytes,schema,openflag); }
Sqlite3::Sqlite3(const void *buffer_including_db, const uint64_t &dbsize_exact, const uint64_t &buffer_including_db_bytes, const char *schema,int openflag) : m(new Sqlite3Internal()){ setup(buffer_including_db,dbsize_exact,buffer_including_db_bytes,schema,openflag); }
Sqlite3::Sqlite3(::sqlite3 *database) : m(new Sqlite3Internal()){ setup(database); }


void Sqlite3::setup(const char *dbpath, int flags) {
    if(sqlite3_open_v2(
            (char *) dbpath
            ,&m->con
            ,flags
            , nullptr) != SQLITE_OK ){
        __error_print;exit(1);
    }
}

void Sqlite3::setup(::sqlite3 *database) {
    m->con = database;
    m->auto_close_ = false;
}

void Sqlite3::setup(const char *dbpath) {
    
    if (sqlite3_open((char *) dbpath, &m->con)) {
        __error_print;exit(1);
    }
    
}

void Sqlite3::setup(const void *ser_data, const uint64_t &ser_data_bytes, const char *schema, int openflag) {
    if(sqlite3_open_v2(":memory:", &m->con, openflag, nullptr) == SQLITE_OK ){
        sqlite3_deserialize(m->con
                            , schema
                            , reinterpret_cast<unsigned char*>(const_cast<void*>(ser_data))
                            , static_cast<int64_t>(ser_data_bytes)
                            , static_cast<int64_t>(ser_data_bytes)
                            , SQLITE_DESERIALIZE_READONLY);
    }else{
        __error_print;exit(1);
    }
    
}

void
Sqlite3::setup(const void *buffer_including_db, const uint64_t &dbsize_exact, const uint64_t &buffer_including_db_bytes, const char *schema, int openflag) {
    if(sqlite3_open_v2(":memory:", &m->con, openflag, nullptr) == SQLITE_OK ){
        sqlite3_deserialize(m->con
                            , schema
                            , reinterpret_cast<unsigned char*>(const_cast<void*>(buffer_including_db))
                            , static_cast<int64_t>(dbsize_exact)
                            , static_cast<int64_t>(buffer_including_db_bytes)
                            , SQLITE_DESERIALIZE_RESIZEABLE);
    }else{
        __error_print;exit(1);
    }
    
}


//ref https://www.sqlite.org/lang_vacuum.html
int Sqlite3::vacuum() { return vacuum(nullptr); }
int Sqlite3::vacuum(const char *file) {
    if(0== bool(m->vcm_on) + m->vcm_off){
        m->vcm_on = compile("PRAGMA auto_vacuum = FULL;");
        m->vcm_off = compile("PRAGMA auto_vacuum = NONE;");
        m->vcm= file ? compile(("vacuum " + m->schema + " into '" + std::string{file} + "'").data(), nullptr) : compile(("vacuum " + m->schema).data(), nullptr); 
    }
    m->vcm_on->step(true);
    auto res= m->vcm->step(false);
    m->vcm_off->step(true);
    return res;
}


int Sqlite3::create_function(
    const char *name
    , int nArg
    , int eTextRep
    , const void * opaque
    , Sqlite3::create_function_f_type fp
    , Sqlite3::create_fucntion_step_f_type step
    , Sqlite3::create_function_final_f_type final
    , Sqlite3::sql_destroy_f_type destroy) {
    sqlite3_create_function_v2(m->con, name, nArg, eTextRep, const_cast<void*>(opaque), fp, step, final, destroy);
    return 0;
}

Sqlite3::~Sqlite3() { delete m; };
::sqlite3 * Sqlite3::connection() { return m->con; }

[[nodiscard]] sqlite3::Serialize * Sqlite3::serializer(const char schema[]){ return sqlite3::sqlite3_serialize(m->con, schema); }

int64_t Sqlite3::rowid_max(const char *table_name,const char col_name_of_id[]) {
    if (!m->row_max) m->row_max = compile((std::string{"select "} + col_name_of_id + " from " + table_name + " order by " + col_name_of_id + " desc limit 1").c_str());
    return m->row_min ? m->row_max->step(false) < SQLITE_ROW ? -1 : sqlite3_column_int64(m->row_max->raw(), 0) : -1;
}
int64_t Sqlite3::rowid_min(const char *table_name,const char col_name_of_id[]) {
    if (!m->row_min)  m->row_min = compile((std::string{"select "} + col_name_of_id  + " from " + table_name + " order by " + col_name_of_id  + " asc limit 1").c_str());
    return m->row_min ?  m->row_min->step(false) < SQLITE_ROW ? -1: sqlite3_column_int64(m->row_min->raw(),0) : -1;
}

bool Sqlite3::send_query(const char query[],const void *parameter,send_query_call_back_t fp_callback) {
    char *tail;
    auto rc = sqlite3_exec(m->con, query, fp_callback, const_cast<void*>(parameter), &tail);
    if (rc != SQLITE_OK) {
        return false;
    } else {
        return true;
    }
};


sqlite3::Options *Sqlite3::options() { return sqlite3::sqlite_options(); }

bool Sqlite3::foreign_key(bool sw){
    static const char* kSentece[2] = {
      "PRAGMA foreign_keys = OFF"      
      ,"PRAGMA foreign_keys = ON"      
    };
    if(!m->foreign_key_sw)m->foreign_key_sw = compile(kSentece[sw]);
    return m->foreign_key_sw ? m->foreign_key_sw->step(true) : false; 
}



Sqlite3Stmt * Sqlite3::compile(const char query[],bool * error, int nByte, const char **pzTail) { return kautil::database::sqlite3_stmt(m->con, query, error, nByte, pzTail); }
Sqlite3Stmt * Sqlite3::compile_allow_asterisk(const char query[], int nByte, const char **pzTail) {
    /// @brief
    /// in my environment, using asterisk as arg of selfdefined function like "select self_defined_function(*)", the amount of column is always 0.
    /// i could't find the way to enable asterisk formally, so wrote CompileAllowAsterisk function
    
    auto org = std::string_view{query};
    auto pos = org.find("*");
    if(pos == std::string::npos) return compile(query,nullptr,nByte, pzTail);
    static  auto kFromAtStr = std::string{" from "};
    auto sv_query = std::string_view{query};
    auto from_at = sv_query.find(" from ");
    if(from_at == std::string::npos) return compile(query,nullptr,nByte, pzTail);

    auto table_name = std::string{sv_query.substr(from_at + kFromAtStr.size())};{
        auto cnt = int(0);
        for(; cnt < table_name.size(); ++cnt)  if(table_name[cnt] == '(' || table_name[cnt] == ' ') break;
        table_name.resize(cnt);
    }

    auto cols = std::string{};{
        auto const q_col = std::string{"select * from "}.append(table_name).append(" limit 1");
        auto stmt = compile(q_col.c_str());
        auto maxi = sqlite3_column_count(stmt->raw());
        for(auto i = 0; i < maxi; ++i){
            cols.append(sqlite3_column_name(stmt->raw(),i)).push_back(',');
        }
        if(!cols.empty()) cols.resize(cols.size() - 1);
        stmt->release();
    }

    auto asterisk = std::string{org.substr(0,pos)} + cols + std::string{org.substr(pos+1,-1)};
    for(;;){
        if( (pos = asterisk.find("*",pos+cols.size())) == std::string::npos) break;
        asterisk = asterisk.substr(0,pos) + cols + asterisk.substr(pos+1,-1);
    }

    return compile(asterisk.data(), nullptr,nByte,pzTail);
}


sqlite3::Alter *Sqlite3::alter(const char *table) { return sqlite3::sqlite3_alter(m->con,table); }
bool Sqlite3::begin_transaction() {
    constexpr char *kQueryBeginTransaction = (char *) "begin transaction;";
    if(!m->begin_transaction) m->begin_transaction = compile(kQueryBeginTransaction);
    return m->begin_transaction ? m->begin_transaction->step(true) : false; 
}
bool Sqlite3::end_transaction() { 
    constexpr char *kQueryCommit = (char *) "commit;";
    if(!m->end_transaction) m->end_transaction = compile(kQueryCommit);
    return m->end_transaction ? m->end_transaction->step(true) : false; 
}
bool Sqlite3::commit() { return end_transaction(); }

bool Sqlite3::roll_back() {
    constexpr char *kQueryRollback = (char *) "rollback;";
    if(!m->roll_back)m->roll_back = compile(kQueryRollback);
    return m->roll_back ? m->roll_back->step(true) : false;
}


} //namespace database {
} //namespace kautil{



sqlite3 * test_data(const char * dbpath,int open_flag){
    auto res = (sqlite3 *) 0;
    if(sqlite3_open_v2(
            (char *) dbpath
            ,&res
            ,open_flag
            , nullptr) != SQLITE_OK ){
        fprintf(stderr,"fail to open database.\n");
        fflush(stderr);
        exit(1);
    }

    {
        char *tail;
        auto rc = sqlite3_exec(res, "create table if not exists test([data] blob,[i64] integer)", kautil::database::send_query_default_call_back, nullptr, &tail);
        if (rc != SQLITE_OK) {
            fprintf(stderr,"fail to create database.\n");
            fflush(stderr);
            exit(1);
        } 
    }
    
    auto stmt = (::sqlite3_stmt *) nullptr;
    if (sqlite3_prepare_v2(res, "insert into test(data,i64) values(?,?)", -1, &stmt, nullptr) != SQLITE_OK){
            fprintf(stderr,"prepair fail. %s\n", sqlite3_errmsg(res));
            fflush(stderr);
            exit(1);
    }
    
    
    auto d =std::string{"data"};
    if(SQLITE_OK!=sqlite3_bind_blob(stmt,1,d.data(), static_cast<int>(d.size()), SQLITE_STATIC)){
        fprintf(stderr,"%s","fail to prepair.");
        fflush(stderr);
    }
    if(SQLITE_OK != sqlite3_bind_int64(stmt,2,123)){
        fprintf(stderr,"%s","fail to prepair.");
        fflush(stderr);
    }
    
    
    if(SQLITE_DONE != sqlite3_step(stmt)){
        fprintf(stderr,"%s", sqlite3_errmsg(res));
        fflush(stderr);
    }
    return res;
}



extern "C" kautil::database::Sqlite3* extern_initialize(){
    return new kautil::database::Sqlite3{};
}
extern "C" void extern_finalize(kautil::database::Sqlite3* m ){
    delete m;
}


int tmain_kautil_sqlite3_shared(){
    auto op = kautil::database::sqlite3::sqlite_options(); // only a partial wrapper of sqlite options. this is for extern module.  
    auto sql = kautil::database::Sqlite3{":memory:"};
    sql.foreign_key(true); // pragma foreign key on
    
    auto fn = sql.file_name();
    auto schema = sql.schema();
    sql.send_query("create table if not exists extra_info([rowid] integer primary key,[ext_data] blob)");

    {// stmt : create table
        if(auto create = sql.compile("create table if not exists test([rowid] integer primary key,[data] blob,[i64] integer)")){
            create->step(true);// true == reset is true
            create->release();
        }else sql.error_msg();
    }

    {// stmt : insert
        if(auto insert = sql.compile("insert into test(data,i64) values(?,?)")){
            for(auto i =0 ; i < 5; ++i){
                insert->set_blob(1,&i,sizeof(i),op->sqlite_static());
                insert->set_int64(2,i);
                insert->step(true);
            }
            insert->release();
        }else sql.error_msg();
    }

    {
        auto minid = sql.rowid_min("test","rowid");
        auto maxid = sql.rowid_max("test","rowid");
        if(minid < 0 || maxid < 0){
            sql.error_msg();
        }
    }

    {// stmt : select
        if(auto select = sql.compile("select data,i64 from test")){
            for(;;){
                auto res = select->step(false);
                if(res != op->sqlite_row()){
                    if(res != op->sqlite_done()) fprintf(stderr, "error : %s", sql.error_string());
                    break;
                }
                auto stmt = select->raw();
                printf("%.*x %lld\n",sqlite3_column_bytes(stmt,0),sqlite3_column_blob(stmt,0),sqlite3_column_int64(stmt,1));
                fflush(stdout);
            }
        }else sql.error_msg();
    }

    
    {// blob
        if(auto blob = sql.blob("main","test","data")){
            for(auto i = rowid_t(1); i < 6;++i){{
                if(blob->is_opened()? !blob->reopen(i) : !blob->open_r(i) ){
                    fprintf(stderr,"error (intended) : %s\n",sql.error_string());
                }
                auto [data,size]=blob->read();
            }}
            blob->release();
        }else sql.error_msg();
    }
    
    
    { // vacuum
        if(auto del = sql.compile("delete from test")){
            del->step(true);
            del->release();
            sql.vacuum();
        }else sql.error_msg();
    }
    
    
    {// alter
        auto alt = sql.alter("test");
        alt->add_column("[summary]","text");
        alt->rename_columns_push("[data]","[detail]");
        alt->rename_columns_push("[summary]","[description]");
        alt->rename_columns(); // execute
        alt->release();
    }
    
    
    {// serialize 
        auto ser = sql.serializer("main");
        ser->serialize();
        auto d = ser->data();
        auto s = ser->size();
        ser->release();
    }
    
    
    op->release();
    
    
    return 0;
}


