
#ifdef TMAIN_KAUTIL_SQLITE_STMT_STATIC

#include "test_data.h"
#include "stdio.h"

int tmain_kautil_sqlite_stmt_static();
int main(){
    auto con = test_data("tmain_kautil_sqlite_stmt_static");
    return tmain_kautil_sqlite_stmt_static(con,printf);
}

#endif