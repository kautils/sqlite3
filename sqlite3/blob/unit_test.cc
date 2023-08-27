


#ifdef TMAIN_KAUTIL_SQLITE3_BLOB

#include "test_data.h"
#include <sqlite3.h>
#include <stdint.h>
#include "c11_string_allocator/c11_string_allocator.h"
#include <string>

#ifdef TMAIN_KAUTIL_SQLITE3_BLOB_STATIC

int tmain_kautil_sqlite3_blob_static(sqlite3 * con    ,void * memman_object
    ,uint64_t (*memman_pos)(void * ptr_of_c11_string)
    ,void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data)
    ,void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos)
);
int main(){
    auto con = test_data("tmain_kautil_sqlite3_blob_static.sqlite");
//    while(true){
//        for(auto i = 0; i< 1000; ++i) {
            auto mem = std::string{};
            auto res =tmain_kautil_sqlite3_blob_static(con, &mem, c11_string_current_pos, c11_string_register, c11_string_pointer);
            if(res) exit(1);
//        }
//    }    
    sqlite3_close(con);
    
}


#endif

#endif