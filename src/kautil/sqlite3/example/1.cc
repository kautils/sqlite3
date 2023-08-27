//
//#include "kautil/sqlite3/sqlite3.h"
//#include "../src/internal/common/internal_arg.h"
//
//#include <cstring>
//#include <filesystem>
//#include <fstream>
//#include <sstream>
//#include <cassert>
//#include <iostream>
//
//
//// assuumed poco
//std::string kBin;
//std::pair<char*,std::string::size_type> get_page( std::string const& url){
//    namespace fs = std::filesystem;
//    kBin.resize(fs::file_size(url));
//    std::ifstream{url,std::ios::binary}.read(kBin.data(), kBin.size());
//    return std::pair{kBin.data(), kBin.size()};
//}
//
//int main(){
//
//
//    // ++++++++++++++++++++++++++++++++++  after filtering ++++++++++++++++++++++++++++++++++
//
//// .. inside  iteration
//    kautil::database::Sqlite3 db("abc.sqlite");
//    auto hw_o = ResultSimplyHired{};
//    hw_o.id = "12345689";
//    hw_o.link_to_detail = "https://www.simplyhired.jp//job/A1h5hoOtxVnspt7tZJtMEdU5TD-NYWQa7xy1Om6-VxPL7I1l2raYLw?q=%22%E3%83%8F%E3%83%AD%E3%83%BC%E3%83%AF%E3%83%BC%E3%82%AF%E6%B1%82%E4%BA%BA%E7%95%AA%E5%8F%B7%22+%22%E7%B4%B9%E4%BB%8B%E6%9C%9F%E9%99%90%E6%97%A5%22";
//    auto [bin,size]  = get_page("R:\\test\\jobsearch\\other\\sqlite3\\hw_rc.html");
//    db.SendQuery("create table if not exists hellowork_detail([id] integer primary key, [hash] blob, [detail] blob);");
//    using kautil::database::sqlite::Blob;
//    db.StepAll("insert or ignore into hellowork_detail(id,hash,detail) values(?,?,?)", {
//            (int64_t) 1, Blob{hw_o.id}, Blob{bin, size}
//    });
//    db.SendQuery("select * from hellowork_detail",nullptr,[](void *custom_parameter, int argc, char **argv, char **azColName){
//        int i;
//        for (i = 0; i < argc; i++) {
//            switch(i){
//                case 2:
//                    std::cout << std::strlen(argv[i]) << std::endl;
//                    break;
//            };
//        }
//        printf("\n");
//        return 0;
//    });
//
//
//
//
//
//    return 0;
//}
//
