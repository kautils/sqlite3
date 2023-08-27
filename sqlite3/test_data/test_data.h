

#ifndef SQLITE3_TEST_DATA_TEST_DATA_H
#define SQLITE3_TEST_DATA_TEST_DATA_H

struct sqlite3;
typedef struct sqlite3 sqlite3;
const char * tbname();
const char * col_rowid_name();
sqlite3 * test_data(const char * dbpath);

#endif