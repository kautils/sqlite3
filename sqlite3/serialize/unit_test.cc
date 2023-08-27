
#ifdef TMAIN_KAUTIL_SQLITE3_BLOB

#include "test_data.h"

#ifdef TMAIN_KAUTIL_SQLITE3_SERIALIZE_STATIC
int tmain_kautil_sqlite3_serialize_static(sqlite3 * db,const char* schema);
int main(){
    auto con = test_data("tmain_kautil_sqlite_serialize.sqlite");
    return tmain_kautil_sqlite3_serialize_static(con,"main");
}

#endif

#endif