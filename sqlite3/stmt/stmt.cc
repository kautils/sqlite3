
#include "stmt.h"
#include <sqlite3.h>


struct kautil::database::Sqlite3StmtInternal{
    friend Sqlite3Stmt * sqlite3_stmt(const char query[],bool * error, int nByte, const char **pzTail);
    sqlite3 * database = 0;
    uint64_t pos=0;
    void * memman_object = 0;
    uint64_t (*memman_pos)(void * ptr_of_c11_string) = 0;
    void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data) = 0;
    void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos) = 0;
};



int kautil::database::Sqlite3Stmt::step(bool reset ) const noexcept{
    auto res =  sqlite3_step(raw());
    if(reset) res = sqlite3_reset(raw());
    return res;
}
int kautil::database::Sqlite3Stmt::reset()noexcept { return sqlite3_reset(raw()); }


using namespace kautil::database;


Sqlite3Stmt * kautil::database::sqlite3_stmt(sqlite3 * dbcon,const char query[],bool * error, int nByte, const char **pzTail
,void * memman_object
,uint64_t (*memman_pos)(void * ptr_of_c11_string)
,void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data)
,void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos)) noexcept{
        auto stmt = (::sqlite3_stmt *) nullptr;
    if (sqlite3_prepare_v2(dbcon, query, nByte, &stmt, pzTail) != SQLITE_OK){
        if(error) *error =true;
        return nullptr;
    }else {
        auto res = new Sqlite3Stmt{};
        {
            res->m->pos = memman_pos(memman_object);
            memman_register(memman_object,res->m->pos,stmt);
            res->m->memman_object = memman_object;
            res->m->memman_pos = memman_pos;
            res->m->memman_register = memman_register;
            res->m->memman_pointer = memman_pointer;
        }
        res->m->database=dbcon;
        if(error) *error =false;
        return res;
    }
    
}



bool kautil::database::Sqlite3Stmt::set_int(int const& i , int const& v )const noexcept{ return sqlite3_bind_int(raw(), i, v) == SQLITE_OK; }
bool kautil::database::Sqlite3Stmt::set_double(const int &i, const double &v)const noexcept { return sqlite3_bind_double(raw(), i, (double) v) == SQLITE_OK; }
bool kautil::database::Sqlite3Stmt::set_int64(int const& i , uint64_t const& v )const noexcept{ return sqlite3_bind_int64(raw(), i, static_cast<int>(v)) == SQLITE_OK; }
bool kautil::database::Sqlite3Stmt::set_text(int const& i , const char * data, uint64_t const& size ,sqlite3_destructor_type lifetime )const noexcept{ return sqlite3_bind_text(raw(), i, data, static_cast<int>(size),lifetime) == SQLITE_OK;  }
bool kautil::database::Sqlite3Stmt::set_blob(const int &i,const void * data, uint64_t size, sqlite3_destructor_type lifetime)const noexcept { return sqlite3_bind_blob(raw(), i, data, static_cast<int>(size), lifetime) == SQLITE_OK; }
bool kautil::database::Sqlite3Stmt::set_null(const int &i,const void * data, uint64_t size, sqlite3_destructor_type lifetime)const noexcept { return sqlite3_bind_null(raw(), static_cast<int>(i)) == SQLITE_OK; }
bool kautil::database::Sqlite3Stmt::set_zeroblob(const int &i, int n)const noexcept{ return sqlite3_bind_zeroblob(raw(),i,n) == SQLITE_OK; }


::sqlite3_stmt * kautil::database::Sqlite3Stmt::raw()const noexcept{
    return reinterpret_cast<::sqlite3_stmt*>(m->memman_pointer(m->memman_object,m->pos));
}


void kautil::database::Sqlite3Stmt::release()noexcept{
    auto stmt = reinterpret_cast<::sqlite3_stmt*>(m->memman_pointer(m->memman_object,m->pos));
    if(stmt){
        sqlite3_finalize(stmt);
        m->memman_register(m->memman_object,m->pos, nullptr);
    }
    delete this;
}


kautil::database::Sqlite3Stmt::Sqlite3Stmt() : m(new Sqlite3StmtInternal){  }
kautil::database::Sqlite3Stmt::~Sqlite3Stmt() { delete m; }


int tmain_kautil_sqlite_stmt_static(sqlite3 * con, int(*__printf)(const char*,...), void * memman_object
        ,uint64_t (*memman_pos)(void * ptr_of_c11_string)
        ,void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data)
        ,void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos)){
    auto create = kautil::database::sqlite3_stmt(con,"create table if not exists test([data] blob,[i64] integer)",nullptr,-1,nullptr,memman_object,memman_pos,memman_register,memman_pointer);
    create->step(true);
    
    auto insert = kautil::database::sqlite3_stmt(con,"insert into test([data],[i64]) values(?,?)",nullptr,-1, nullptr,memman_object,memman_pos,memman_register,memman_pointer);
    const char *data = "some";
    auto i64 = 123;
    if(insert){ 
        insert->set_blob(1,data,4);
        insert->set_int64(2,i64);
        insert->step(true);
    
        auto select = kautil::database::sqlite3_stmt(con,"select * from test"
                                                     ,nullptr
                                                     ,-1
                                                     , nullptr
                                                     ,memman_object
                                                     ,memman_pos
                                                     ,memman_register
                                                     ,memman_pointer
                                                     );
        for(;;){
            auto res = select->step(false);
            if(res != SQLITE_ROW){
                if(res != SQLITE_DONE)sqlite3_errmsg(con);
                break;
            }
            auto raw = select->raw();
            auto data = sqlite3_column_blob(raw,0);
            auto data_len = sqlite3_column_bytes(raw,0);
            auto i64 = sqlite3_column_int64(raw,1);
            __printf("data(%.*s) i64(%lld)\n",data_len,(char*)data,i64);
        }
        create->release();
        insert->release();
        select->release();
        return 0;
    }else{
        return -1;
    }

    
}

