#include "kautil/sqlite3/sqlite3.h"
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include "sqlite3.h"
int main(){

    kautil::database::Sqlite3 db(":memory:");
    db.SendQuery(""
                 "create table blob_tb("
                     "[id] integer primary key,"
                     "[int1] integer ,"
                     "[str1] text ,"
                     "[blob1] blob ,"
                     "[real1] real"
                 ")");

    using namespace std::chrono_literals;
    for(auto int1 : std::vector<int>{1,2,3,4,5}){
        for(auto str1 : std::vector<std::string>{"time_length","b","c","d","e"}){
            for(auto blob1 : std::vector<std::chrono::milliseconds>{1ms,2ms,3ms,4ms,5ms}){
                for(auto real1 : std::vector<double>{0.1,0.2,0.3,0.4,0.5}){
                    static auto stmt_insert_row = db.Compile("insert or replace into blob_tb(int1,str1,blob1,real1) values(?,?,?,?)");
                    db.SetInt(&stmt_insert_row,1,int1);
                    db.SetText(&stmt_insert_row,2,str1.data(),str1.size());
                    db.SetBlob(&stmt_insert_row,3,&blob1,sizeof(blob1));
                    db.SetDouble(&stmt_insert_row,4,real1);
                    if(db.Step(&stmt_insert_row) != SQLITE_DONE){
                        db.error_msg();
                    }
                }
            }
        }
    }


    using std::chrono::milliseconds;
    kautil::database::Sqlite3BlobRW tb(db.database(), "blob_tb");
    auto blob = tb.Open(1,3);
    *(milliseconds * )blob.data() = 100ms;
    tb.Write(&blob);
    tb.Read(&blob);
    std::cout << (*(milliseconds * )blob.data()).count() << std::endl;






}