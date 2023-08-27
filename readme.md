### kautil_sqlite3
* partial wrapper of sqlite3.
* mingw.
* can extern "C" via dlopen/LoadLibrary

### note
* there may be a lot of misstakes.

### example 
```c++

int main(){
    auto op = kautil::database::sqlite3::sqlite_options(); // only a partial wrapper of sqlite options. this is for extern module.  
    auto sql = kautil::database::Sqlite3{":memory:"};
    sql.foreign_key(true); // pragma foreign key on
    
    auto fn = sql.file_name();
    auto schema = sql.schema();
    sql.send_query("create table if not exists extra_info([rowid] integer primary key,[ext_data] blob)");

    {// stmt : create table
        if(auto create = sql.compile("create table if not [readme.md](..%2Fbuf%2Freadme.md)exists test([rowid] integer primary key,[data] blob,[i64] integer)")){
            create->step(true);// true == reset is true
            create->release();
        }else sql.error_msg();
    }

    {// stmt : insert
        if(auto insert = sql.compile("insert into test(data,i64) values(?,?)")){
            for(auto i =0 ; i < 5; ++i){
                insert->set_blob(1,[readme.md](..%2Fbuf%2Freadme.md)&i,sizeof(i),op->sqlite_static());
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



```
