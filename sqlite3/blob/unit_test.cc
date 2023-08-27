


#ifdef TMAIN_KAUTIL_SQLITE3_BLOB

#include "test_data.h"


#ifdef TMAIN_KAUTIL_SQLITE3_BLOB_STATIC

int tmain_kautil_sqlite3_blob_static(sqlite3 * con);
int main(){
    auto con = test_data("tmain_kautil_sqlite3_blob_static.sqlite");    
    return tmain_kautil_sqlite3_blob_static(con);
}


#endif

#endif