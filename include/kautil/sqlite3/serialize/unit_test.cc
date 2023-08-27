#ifdef TMAIN_KAUTIL_SQLITE_SERIALIZE

#include <sqlite3.h>
#include "kautil/sqlite3/sqlite3.h"
int tmain_kautil_sqlite_serialize(sqlite3 * db,const char* scheme);

int main(){
    using db_t = kautil::database::Sqlite3;
    auto sql=db_t{":memory:",SQLITE_OPEN_READWRITE|SQLITE_OPEN_NOMUTEX};
    sql.SendQuery("create table if not exists some_table([rowid] integer primary key,[data] blob)");
    return tmain_kautil_sqlite_serialize(sql.database(),sql.scheme());
}

#endif
