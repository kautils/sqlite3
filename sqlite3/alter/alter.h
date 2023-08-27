#ifndef KAUTIL_SQLITE_SQLITE3_ALTER_ALTER_H
#define  KAUTIL_SQLITE_SQLITE3_ALTER_ALTER_H

struct sqlite3;
typedef sqlite3 sqlite3;

namespace kautil{
namespace database {
namespace sqlite3{


struct AlterInternal;
struct Alter {
    friend Alter * sqlite3_alter(::sqlite3 * db, const char table[]);
    virtual ~Alter();
    virtual bool update_table_info();
    virtual bool add_column(const char  name[], const char definition[]);
    virtual bool delete_column(const char  name[]);
    virtual void rename_columns_push(const char * from, const char * to);
    virtual bool rename_columns();
    virtual void release();
    
private:
    Alter(::sqlite3 * db, const char table[]);
    AlterInternal * m_ = nullptr;
};

Alter * sqlite3_alter(::sqlite3 * db, const char table[]);


} //namespace sqlite3{
} //namespace database{
} //namespace kautil{




#endif