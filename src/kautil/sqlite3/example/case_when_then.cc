


#include "kautil/sqlite3/sqlite3.h"
#include "sqlite3.h"
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <cassert>



static auto state_default = uint64_t(0);
static  auto state_trash = uint64_t(1);


double  main1(kautil::database::Sqlite3 & sql){
    {
        auto update_case_when_then = sql.Compile("update blob "
                                                     " set shared = decriment_shared(shared),"
                                                     " state  = case"
                                                                     " when is_shared_empty(shared) not null"
                                                                         " then ? else ?"
                                                                     " end"
                                                 " where is_shared_empty(shared) is null"

                                                 );

        using sysc = std::chrono::system_clock;
        auto s = sysc::now();
        sql.SetBlob(&update_case_when_then,1,&state_default,sizeof( state_default),kautil::database::life_static);
        sql.SetBlob(&update_case_when_then,2,&state_trash,sizeof( state_trash),kautil::database::life_static);
        sql.Step(&update_case_when_then,false);
        return static_cast<double>((sysc::now() - s).count());
    }
}


/* faster  */
double main2(kautil::database::Sqlite3 & sql){
    auto update_case_when_then = sql.Compile("update blob "
                                                 " set shared = decriment_shared(shared),"
                                                     " state  = a(shared)"
                                             " where is_shared_empty(shared) is null"

                                             );

    using sysc = std::chrono::system_clock;
    auto s = sysc::now();
    sql.Step(&update_case_when_then,false);
    return static_cast<double>((sysc::now() - s).count());
}


double process(double(*func)(kautil::database::Sqlite3 &),const char *path_to_sql){
    auto sql = kautil::database::Sqlite3{path_to_sql};
    if(!sql.SendQuery("create table if not exists blob([rowid] integer primary key,[shared] blob,[state] blob)")){
        sql.error_msg();
        assert(false);
        return 0;
    }else{
        sql.BeginTransaction();
        auto insert = sql.Compile("insert or replace into blob(shared,state) values(?,?)");
        for(auto i = uint64_t(0); i < 100; ++i){
            auto shared = static_cast<uint64_t>(rand() % 5);
            auto state = static_cast<uint64_t>(!shared ? 0 : 1);
            sql.SetBlob(&insert,1,&shared,sizeof(shared));
            sql.SetBlob(&insert,2,&state,sizeof(state));
            if(sql.Step(&insert,true) != SQLITE_DONE){
                sql.error_msg();
                assert(false);
                return 0;
            }
        }
        sql.Commit();

        // https://stackoverflow.com/questions/17079697/update-command-with-case-in-sqlite
        struct opqque{} opq;
        sql.CreateFunction("a",-1,SQLITE_DETERMINISTIC,&opq,[](auto c,auto n, auto v){
            static auto res_0 = uint64_t(0);
            static auto res_1 = uint64_t(1);
            auto const& current = *reinterpret_cast<const uint64_t*>(sqlite3_value_blob(v[0]));
            if( !current || !(current-1)){
                sqlite3_result_blob(c,&res_1,sizeof(res_1),SQLITE_TRANSIENT);
            }else{
                sqlite3_result_blob(c,&res_0,sizeof(res_0),SQLITE_TRANSIENT);
            }
        });
        sql.CreateFunction("decriment_shared",-1,SQLITE_DETERMINISTIC,&opq,[](auto c,auto n, auto v){
            static uint64_t res;
            res = *reinterpret_cast<const uint64_t*>(sqlite3_value_blob(v[0]));
            if(res) res -=1;
            sqlite3_result_blob(c,&res,sizeof(res),SQLITE_STATIC);
        });
        sql.CreateFunction("is_shared_empty",-1,SQLITE_DETERMINISTIC,&opq,[](auto c,auto n, auto v){
            if( *reinterpret_cast<const uint64_t*>(sqlite3_value_blob(v[0])) == 0){
                static auto const& b= true;
                sqlite3_result_blob(c,&b,sizeof(b),SQLITE_STATIC);
            }else{
                sqlite3_result_null(c);
            }
        });
        sql.BeginTransaction();
        auto res = func(sql);
        sql.Commit();
        return res;
    }

    return 1;
}

int main(){

    srand((unsigned int) std::chrono::system_clock::now().time_since_epoch().count());
    namespace fs = std::filesystem;
    if(fs::exists("x.sqlite")) fs::remove("x.sqlite");
    if(fs::exists("y.sqlite")) fs::remove("y.sqlite");

    for(auto i = 0; i < 10; ++i)
        std::cout << std::setprecision(10) << (FindKeyWordProcess(main1, "x.sqlite") / FindKeyWordProcess(main2, "y.sqlite")) << std::endl;

//    process(main2,"y.sqlite");

    return 0;
}