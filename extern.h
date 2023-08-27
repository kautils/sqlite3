#ifndef KAUTIL_SQLITE3_EXTERN_EXTERN_H
#define KAUTIL_SQLITE3_EXTERN_EXTERN_H
#include "sqlite3/sqlite3.h"
kautil::database::Sqlite3 * kautil_database_sqlite3_initialize();
void  kautil_database_sqlite3_release(kautil::database::Sqlite3 * m);


#endif