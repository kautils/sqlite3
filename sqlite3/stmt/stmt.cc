
#include "stmt.h"
#include <sqlite3.h>


struct kautil::database::Sqlite3StmtInternal{
    friend Sqlite3Stmt * sqlite3_stmt(const char query[],bool * error, int nByte, const char **pzTail);
    ::sqlite3_stmt * stmt = 0;
    sqlite3 * database = 0;
    uint64_t pos=0;
};



int kautil::database::Sqlite3Stmt::step(bool reset ) const noexcept{
    auto res =  sqlite3_step(raw());
    if(reset) res = sqlite3_reset(raw());
    return res;
}
int kautil::database::Sqlite3Stmt::reset()noexcept { return sqlite3_reset(raw()); }


using namespace kautil::database;
Sqlite3Stmt * kautil::database::sqlite3_stmt(sqlite3 * dbcon
                                             ,const char query[]
                                             ,bool * error
                                             , int nByte
                                             , const char **pzTail
                                             ) noexcept {
    auto stmt = (::sqlite3_stmt *) nullptr;
    if (sqlite3_prepare_v2(dbcon, query, nByte, &stmt, pzTail) != SQLITE_OK){
        if(error) *error =true;
        return nullptr;
    }else {
        Sqlite3Stmt * res = new Sqlite3Stmt{};
        res->m->database=dbcon;
        res->m->stmt = stmt;
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
    return m->stmt;
}


void kautil::database::Sqlite3Stmt::release()noexcept{
    if(m->stmt){
        sqlite3_finalize(m->stmt);
        m->stmt=0;
    }
    delete this;
}


kautil::database::Sqlite3Stmt::Sqlite3Stmt() : m(new Sqlite3StmtInternal){  }
kautil::database::Sqlite3Stmt::~Sqlite3Stmt() { delete m; }



int tmain_kautil_sqlite_stmt_static(sqlite3 * con,int(*__printf)(const char * fmt,...)){
//        auto con = test_data("tmain_kautil_sqlite_stmt_static.sqlite",SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE);
    auto create = kautil::database::sqlite3_stmt(con,"create table if not exists test([data] blob,[i64] integer)",nullptr,-1,nullptr);
    create->step(true);
    
    auto insert = kautil::database::sqlite3_stmt(con,"insert into test([data],[i64]) values(?,?)",nullptr,-1, nullptr);
    const char *data = "some";
    auto i64 = 123;
    if(insert){ 
        insert->set_blob(1,data,4);
        insert->set_int64(2,i64);
        insert->step(true);
    
        auto select = kautil::database::sqlite3_stmt(con,"select * from test", nullptr,-1, nullptr);
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

