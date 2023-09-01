

#ifdef TMAIN_KAUTIL_SQLITE_STMT_STATIC

#include "sqlite3.h"
#include "test_data.h"
#include "stdio.h"
#include <stdint.h>
#include <string>
#include "c11_string_allocator/c11_string_allocator.h"

int tmain_kautil_sqlite_stmt_static(sqlite3 * con, int (*__printf)(const char*,...), void * memman_object
        ,uint64_t (*memman_pos)(void * ptr_of_c11_string)
        ,void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data)
        ,void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos)) ;
int main(){
    auto memobj = std::string{}; 
    auto con = test_data("tmain_kautil_sqlite_stmt_static");
    return tmain_kautil_sqlite_stmt_static(con,printf,&memobj,c11_string_current_pos, c11_string_register, c11_string_pointer); 
}

#elif defined(TMAIN_KAUTIL_SQLITE_STMT_STATIC_INTERFACE_TEST)

#include "sqlite3.h"
#include "stmt.h"


void aaa(){
    
}


//int kautil::database::Sqlite3Stmt::step(bool reset ) const noexcept{
//    auto res =  sqlite3_step(raw());
//    if(reset) res = sqlite3_reset(raw());
//    return res;
//}
//


//int main(){
//    auto sql =  (sqlite3 *) 0;
//    sqlite3_open(":memory:",&sql);
//    int jjj= 0;
//    return 0;
//}
//


#endif
