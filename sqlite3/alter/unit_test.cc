

#ifdef TMAIN_KAUTIL_SQLITE3_ALTER
#include "test_data.h"

#ifdef TMAIN_KAUTIL_SQLITE3_ALTER_STATIC
int tmain_kautil_sqlite3_alter_static(sqlite3 * con);
int main(){
    auto con = test_data("tmain_kautil_sqlite3_alter_static.sqlite");
    return tmain_kautil_sqlite3_alter_static(con);
}

#endif


#endif