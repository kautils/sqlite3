//https://sqlite.org/loadext.html : want to use
//The carray() function is not compiled into SQLite by default. It is available as a loadable extension in the ext/misc/carray.c source file.
//The carray() function was first added to SQLite in version 3.14 (2016-08-08). The sqlite3_carray_bind() interface and the single-argument variant of carray() was added in SQLite version 3.34.0 (2020-12-01). The ability to bind an array of struct iovec objects that are interpreted as BLOBs was added in SQLite version 3.41.0 (2023-02-21).

//
//
//#include "kautil/sqlite3/sqlite3.h"
//#include "sqlite3.h"
//#include <iostream>
//
//int main(){
//
//    auto sql = kautil::database::Sqlite3{":memory:"};
//    {
//        sql.SendQuery("create table if not exists carray_test([rowid] interger primary key,[data] blob)");
//        auto stmt = sql.Compile("insert or ignore into carray_test(blob) values(?)");
//        for(auto i = 0; i < 100;++i){
//            sql.SetBlob(&stmt,1,&i,sizeof(i));
//        }
//    }
//
//    {
//        auto stmt = sql.Compile("select data from carray_test where id in (?)");
//        for(;;){
//            if(sql.Step(&stmt,false) == SQLITE_DONE || sql.Step(&stmt,false) != SQLITE_ROW){
//                sql.error_msg();
//                break;
//            }
//            std::cout << "A" << std::endl;
//        }
//    }
//
//
//
//
//
//
//    return 0;
//}