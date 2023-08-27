

#include "test_data.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>


#define TBNAME "test"
#define COL_ROW_ID_NAME "rowid"


const char * tbname(){ return TBNAME; }
const char * col_rowid_name(){ return COL_ROW_ID_NAME; }

sqlite3 * test_data(const char * dbpath){
    
    auto data_len = []<int size>(auto (&)[size]){ return size; };
    
    auto res = (sqlite3 *) 0;
    if(sqlite3_open_v2(
            (char *) dbpath
            ,&res
            ,SQLITE_OPEN_CREATE|SQLITE_OPEN_READWRITE
            , nullptr) != SQLITE_OK ){
        fprintf(stderr,"fail to open database.\n");
        fflush(stderr);
        exit(1);
    }

    {
        char *tail;
        auto rc = sqlite3_exec(res, "create table if not exists " TBNAME "([" COL_ROW_ID_NAME "] integer primary key,[data] blob,[i64] integer)", [](auto ,auto,auto,auto){ return 0;}, nullptr, &tail);
        if (rc != SQLITE_OK) {
            fprintf(stderr,"fail to create database.\n");
            fflush(stderr);
            exit(1);
        } 
    }
    
    auto stmt = (::sqlite3_stmt *) nullptr;
    if (sqlite3_prepare_v2(res, "insert into " TBNAME "(data,i64) values(?,?)", -1, &stmt, nullptr) != SQLITE_OK){
            fprintf(stderr,"prepair fail. %s\n", sqlite3_errmsg(res));
            fflush(stderr);
            exit(1);
    }
    
    
    const char d[]="data";
    if(SQLITE_OK!=sqlite3_bind_blob(stmt,1,d, data_len(d), SQLITE_STATIC)){
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

