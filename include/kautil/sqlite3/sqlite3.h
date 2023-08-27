

#ifndef _MNT_NVME0N1_GIT_LOCAL_2_LOCAL_DAEMON_FIFO_TIPS_DIRTY_SQLITE_GEN_GEN_H
#define _MNT_NVME0N1_GIT_LOCAL_2_LOCAL_DAEMON_FIFO_TIPS_DIRTY_SQLITE_GEN_GEN_H

//#define to_stmt(v) reinterpret_cast<sqlite3_stmt*>(v.ptr)
#define to_stmt(obj) obj.stmt()
#define to_sqlite3_blob(v) (sqlite3_blob*)v.ptr

#include <stdint.h>

struct sqlite3;
typedef struct sqlite3 sqlite3;

struct sqlite3_stmt;
typedef struct sqlite3_stmt sqlite3_stmt;

struct sqlite3_context;
struct sqlite3_value;
using rowid_t = int64_t;

namespace kautil{
namespace database {


struct Sqlite3StmtInternal;
struct Sqlite3Stmt{
    Sqlite3Stmt();
    ~Sqlite3Stmt();
    
    Sqlite3Stmt(Sqlite3Stmt &&);
    Sqlite3Stmt & operator=(Sqlite3Stmt && v);
    Sqlite3Stmt(Sqlite3Stmt const&)=delete;
    
    sqlite3_stmt * stmt();
    void release();
    
    Sqlite3StmtInternal * m = 0;
};


struct Sqlite3DataBase{ void * ptr = nullptr; };


extern int kSQLITE_DETERMINISTIC;
extern int kSQLITE_DIRECTONLY;
extern int kSQLITE_SUBTYPE;
extern int kSQLITE_INNOCUOUS;
extern int  kSQLITE_UTF8 ;
extern int  kSQLITE_UTF16LE;
extern int  kSQLITE_UTF16BE;
extern int  kSQLITE_UTF16;
extern int  kSQLITE_ANY;
extern int  kSQLITE_UTF16_ALIGNED;



int DefaultCallBack(void *NotUsed, int argc, char **argv, char **azColName);
enum Sqlite3Lifetime{ kStatic,kTrainsient };



using sqlite3_life = Sqlite3Lifetime;
inline static const sqlite3_life life_static =Sqlite3Lifetime::kStatic;
inline static const sqlite3_life life_transit =Sqlite3Lifetime::kTrainsient;


enum Sqlite3CompileOption{ kSpeed,kAllowAsterisk };
using sqlite3_comple_option = Sqlite3CompileOption;
inline static const sqlite3_comple_option speed = sqlite3_comple_option ::kSpeed;
inline static const sqlite3_comple_option allow_asterisk =sqlite3_comple_option ::kAllowAsterisk;


struct Sqlite3Internal;
class Sqlite3 {
    Sqlite3Internal *  m = nullptr;

    
public:
    
    using sql_function_type = void (*)(sqlite3_context *context, int argc, sqlite3_value **argv);
    using sql_step_function_type = void (*)(sqlite3_context*,int,sqlite3_value**);
    using sql_final_function_type = void (*)(sqlite3_context*);
    using sql_destroy_function_type = void(*)(void*);
    int CreateFunction(
            const char *name
            , int nArg
            , int eTextRep
            ,const void * opaque
            ,sql_function_type fp
            ,sql_step_function_type step = nullptr
            ,sql_final_function_type final = nullptr
            ,sql_destroy_function_type destroy = nullptr
        );

    explicit Sqlite3(sqlite3* database); /// @note use existing. if initialized via this constructor, automatical close dose not happen.
    explicit Sqlite3(const char dbpath[]);

    // open read only
    Sqlite3(const void * ser_data,uint64_t const& ser_data_bytes,const char * scheme,int openflag );
    // open resizable
    Sqlite3(const void * buffer_including_db,uint64_t const& dbsize_exact,uint64_t const& buffer_including_db_bytes,const char * scheme,int openflag);
    
    
    ///@note pragma of vacuume is 'FULL' 
    ///@ref https://www.sqlite.org/lang_vacuum.html
    int vacuum();
    int vacuum(const char * file);
    
    
    // serialize with nocopy
    uint64_t serialize(void ** res);
    
    ///@note responsible for free 'res'
    uint64_t serialize_detatch(void ** res);

    // sqlite3_open_v2
    Sqlite3(const char dbpath[],int flags);
    virtual ~Sqlite3();
    sqlite3 * database();
    const char * scheme();

    void SetVerbose(bool v);
    bool SendQuery(const char query[],const void *parameter = nullptr, decltype(DefaultCallBack) fp_callback = DefaultCallBack);
    Sqlite3Stmt Compile(const char query[], int nByte = -1, const char **pzTail = nullptr);
    Sqlite3Stmt Compile(const char query[],bool * error, int nByte = -1, const char **pzTail = nullptr);

    /// @brief
    /// in my environment, using asterisk as arg of selfdefined function like "select self_defined_function(*)", the amount of column is always 0.
    /// i could't find the way to enable asterisk formally, so wrote CompileAllowAsterisk function
    Sqlite3Stmt CompileAllowAsterisk(const char query[], int nByte = -1, const char **pzTail = nullptr);

    ///@brief do from compile to finalize
//    int Finalize(Sqlite3Stmt *stmt);
    bool SetInt(Sqlite3Stmt * stmt, int const& i , int const& v );
    bool SetDouble(Sqlite3Stmt * stmt, int const& i , double const& v );
    bool SetInt64(Sqlite3Stmt * stmt, int const& i , uint64_t const& v );
    bool SetText(Sqlite3Stmt * stmt, int const& i ,const char * data, uint64_t const& size ,sqlite3_life lifetime = life_static );
    bool SetBlob(Sqlite3Stmt * stmt, int const& i ,const void *, uint64_t size,sqlite3_life lifetime = life_static );
    bool SetZeroBlob(Sqlite3Stmt * stmt, const int &i, int);
    bool SetNull(Sqlite3Stmt * stmt, int const& i ,const void *, uint64_t size,sqlite3_life lifetime = life_static );

    const char * FileName();


    int Step(Sqlite3Stmt *stmt, bool reset);
    int StepAll(Sqlite3Stmt *stmt);
    bool Reset(Sqlite3Stmt *stmt);
    
    
    Sqlite3Stmt RowidMaxStmt(const char table_name[],const char col_name_of_id[]); 
    Sqlite3Stmt RowidMinStmt(const char  table_name[],const char col_name_of_id[]); 
    

    uint64_t RowidMax(const char table_name[],const char col_name_of_id[]);
    uint64_t RowidMin(const char  table_name[],const char col_name_of_id[]);


    bool SendQueryWithFile(const char dbpath[],const void *parameter = nullptr, decltype(DefaultCallBack) fp_callback = DefaultCallBack);
    bool BeginTransaction();

    bool EndTransaction(); /// @note equal to Commit
    bool Commit();/// @note equal to EndTransaction
    bool Rollback();

    /// @return binary data in the column , if fail return nullptr
    /// @note row_id is not index, starting from  1  
//    std::unique_ptr<std::vector<uint8_t>>
//    ReadBlob(const char table_name[], std::string const &col_name, int64_t row_id, DataBaseMode db_mode = kMain);

    bool error_msg();
    const char * error_string();

private:


}; // class








// ++++++++++++++++++++++++++++++++++++++++++++ Sqlite3BlobRW ++++++++++++++++++++++++++++++++++++++++++++

#ifndef to_sqlite3_blob
#define to_sqlite3_blob(v) (sqlite3_blob*)v.ptr()
#endif

struct Sqlite3BlobInternal;
class Sqlite3Blob final{

    friend class Sqlite3BlobRW;
public:
    Sqlite3Blob();
    ~Sqlite3Blob();
    Sqlite3Blob(Sqlite3Blob && v);
    Sqlite3Blob & operator=(Sqlite3Blob && v);
    Sqlite3Blob(Sqlite3Blob const&)= delete;
    Sqlite3Blob & operator=(Sqlite3Blob const&)= delete;


//    operator bool();


    bool IsOpened();
    void * ptr();
    void * data();
    unsigned long long int size();


private:

    /// @note use to express null
    //// make it fast in case of the error 'no such rowid' by ommitting new/delete Sqlite3BlobInternal
    Sqlite3Blob(bool);
    Sqlite3Blob(void *,int const& );
    Sqlite3BlobInternal * m_ = nullptr;


};



struct Sqlite3BlobRWInternal;
class Sqlite3BlobRW final{

public:

    Sqlite3BlobRW(void * sql, const char table[], bool temp = false);
    ~Sqlite3BlobRW();
    void err_msg();
    int UpdateCol();
    bool Write(Sqlite3Blob * rp_blob,int const& offset = 0);
    bool Write(Sqlite3Blob * rp_blob,const void * data,uint64_t const& size ,int const& offset = 0);
    Sqlite3Blob Open(int64_t const& row , char const col_name[],int flag = 1);
    bool Read(Sqlite3Blob * rp_blob,int const& offset = 0);
//    Sqlite3Blob Open(int64_t const& row ,unsigned long long const& col,int flag = 1);
//    Sqlite3Blob OpenRW(int64_t const& row ,unsigned long long const& col);
    Sqlite3Blob OpenRW(int64_t const& row ,const char col[]);
//    Sqlite3Blob OpenR(int64_t const& row ,unsigned long long const& col);
    Sqlite3Blob OpenR(int64_t const& row ,char const col[]);

    int LastResult();
    /// @note
    /// make it a little complicated in implementation
    /// always check not null for the target column
    /// less improvement about speed in my environment comparing with opening repeatedly blob object
    bool ReOpen(Sqlite3Blob * blob_obj ,long long const& row);

    int const& last_state();



    int ColCount();
    int Type(unsigned long const& i );
    const char * Name(unsigned long const& i );




private:
    Sqlite3BlobRWInternal * m_ = nullptr;
};


// ++++++++++++++++++++++++++++++++++++++++++++ Sqlite3BlobRW ++++++++++++++++++++++++++++++++++++++++++++




struct RenameElementInternal;
class RenameElement{
    RenameElement(const char * from,const char * to);
    RenameElement();
public:
    ~RenameElement();
    friend class Sqlite3AlterTable;
    static RenameElement make_pair(const char * from,const char * to);
    RenameElement& operator()(const char * from,const char * to);
private:
    RenameElementInternal *  m_ = nullptr;
};


struct Sqlite3AlterTableInternal;
class Sqlite3AlterTable final{

public:

    Sqlite3AlterTable(void * db,const char table[]);
    ~Sqlite3AlterTable();
    int UpdateTable();
    int AddColumn(const char  name[], const char definition[]);
    int DeleteColumn(const char  name[]);
    int RenameColumns(RenameElement const& v);

private:

    Sqlite3AlterTableInternal * m_ = nullptr;
};

using fp_sqlite3_alter_pair =  RenameElement (*)(const char * from,const char * to);
static fp_sqlite3_alter_pair sqlite3_alter_col_arg = RenameElement::make_pair;




} //namespace database{
} //namespace kautil{

/// @return success        : SQLITE_DONE
/// @return already exists : SQLITE_ERROR
/// @note use when not need save col name into internal map
int kautil_sqlite3_add_column(sqlite3 * db,const char * table,const char  name[], const char definition[]);

#include "./blob.h"
#include "./serialize/serialize.h"

#endif