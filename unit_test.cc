#ifdef TMAIN_KAUTIL_SQLITE3_EXTERN_STATIC

int tmain_kautil_sqlite3_extern_static();
int main(){
    return tmain_kautil_sqlite3_extern_static();
}

#include "extern.h"
int tmain_kautil_sqlite3_extern_static(){
    auto m = kautil_database_sqlite3_initialize();
    auto op = m->options();
    m->setup(":memory:");
    m->send_query("create table if not exists extern_test([rowid] integer primary key,[data] blob)", nullptr,[](auto,auto,auto,auto){return 0;});
    if(auto insert = m->compile("insert into extern_test(data) values(?)")){
        const char *data = "abcd";
        op->sqlite_static();
        insert->set_blob(1,data,2,op->sqlite_static());
        insert->release();
    }
    op->release();
    kautil_database_sqlite3_release(m);
    return 0;
}


#endif