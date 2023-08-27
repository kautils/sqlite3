

#ifndef KAUTIL_DATABASE_SQLITE3
#define KAUTIL_DATABASE_SQLITE3

#include <stdint.h>
#include "./stmt/stmt.h"
#include "./blob/blob.h"
#include "./preprocessors/preprocessors.h"
#include "./serialize/serialize.h"
#include "./alter/alter.h"

struct sqlite3;
typedef struct sqlite3 sqlite3;

struct sqlite3_context;
struct sqlite3_value;
using rowid_t = int64_t;

namespace kautil{
namespace database {

int send_query_default_call_back(void *NotUsed, int argc, char **argv, char **azColName);
struct Sqlite3Internal;
struct Sqlite3 {
    
    using send_query_call_back_t = int (*)(void *NotUsed, int argc, char **argv, char **azColName);
    using create_function_f_type = void (*)(sqlite3_context *context, int argc, sqlite3_value **argv);
    using create_fucntion_step_f_type = void (*)(sqlite3_context*, int, sqlite3_value**);
    using create_function_final_f_type = void (*)(sqlite3_context*);
    using sql_destroy_f_type = void(*)(void*);
    
    virtual int create_function(
            const char *name
            , int nArg
            , int eTextRep
            , const void * opaque
            , create_function_f_type fp
            , create_fucntion_step_f_type step = nullptr
            , create_function_final_f_type final = nullptr
            , sql_destroy_f_type destroy = nullptr
        );
    
    explicit Sqlite3(); /// @note use existing. if initialized via this constructor, automatical close dose not happen.
    virtual ~Sqlite3();
    Sqlite3(const char dbpath[],int flags); ///@note sqlite3_open_v2
    explicit Sqlite3(::sqlite3* database); /// @note use existing. if initialized via this constructor, automatical close dose not happen.
    explicit Sqlite3(const char dbpath[]);
    Sqlite3(const void * ser_data,uint64_t const& ser_data_bytes,const char * schema,int openflag ); ///@note open read only
    Sqlite3(const void * buffer_including_db,uint64_t const& dbsize_exact,uint64_t const& buffer_including_db_bytes,const char * schema,int openflag); ///@note open resizable
    
    virtual void setup(const char dbpath[],int flags); ///@note sqlite3_open_v2
    virtual void setup(::sqlite3* database); /// @note use existing. if initialized via this constructor, automatical close dose not happen.
    virtual void setup(const char dbpath[]);
    virtual void setup(const void * ser_data,uint64_t const& ser_data_bytes,const char * schema,int openflag ); ///@note open read only
    virtual void setup(const void * buffer_including_db,uint64_t const& dbsize_exact,uint64_t const& buffer_including_db_bytes,const char * schema,int openflag); ///@note open resizable
    
    
    
    virtual int vacuum();///@note pragma of vacuume is 'FULL' 
    virtual int vacuum(const char * file);
    virtual bool foreign_key(bool sw);
    
    virtual ::sqlite3 * connection();
    virtual const char * schema();
    virtual bool send_query(const char query[],const void *parameter = nullptr, send_query_call_back_t fp_callback = send_query_default_call_back);
    
    [[nodiscard]] virtual Sqlite3Stmt * compile(const char query[],bool * error = nullptr, int nByte = -1, const char **pzTail = nullptr);
    [[nodiscard]] virtual Sqlite3Stmt * compile_allow_asterisk(const char query[], int nByte = -1, const char **pzTail = nullptr);
    [[nodiscard]] virtual sqlite3::Blob* blob(const char schema[], const char table[],const char colname[]);
    [[nodiscard]] virtual sqlite3::Serialize * serializer(const char schema[]);
    [[nodiscard]] virtual sqlite3::Alter * alter(const char table[]);
    [[nodiscard]] virtual sqlite3::Options * options();
    
    virtual const char * file_name();
    virtual rowid_t rowid_max(const char table_name[],const char col_name_of_id[]);
    virtual rowid_t rowid_min(const char  table_name[],const char col_name_of_id[]);
    
    virtual bool begin_transaction();
    virtual bool end_transaction(); /// @note equal to commit
    virtual bool commit();
    virtual bool roll_back();
    virtual bool error_msg();
    virtual const char * error_string();
private:
    Sqlite3Internal *  m = nullptr;
}; // class


} //namespace database{
} //namespace kautil{



#endif