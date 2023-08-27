

#ifndef KAUTIL_SQLITE3_BLOB_H
#define KAUTIL_SQLITE3_BLOB_H

struct sqlite3;
typedef struct sqlite3 sqlite3;

struct sqlite3_blob;
typedef struct sqlite3_blob sqlite3_blob;


namespace kautil{
namespace database{
namespace sqlite3{


using sqlite_rowid_t = int64_t;
struct BlobInternal;
struct Blob{
    friend struct BlobInternal;
public :
    
    struct read_reult{ void* data = 0; int32_t size = 0; };
    Blob();
    ~Blob();
    virtual bool setup(::sqlite3 * database,const char schema[], const char table[],const char colname[]);
    virtual bool is_opened();
    virtual bool open_r(sqlite_rowid_t const& rowid);
    virtual bool open_rw(sqlite_rowid_t const& rowid);
    virtual bool reopen(sqlite_rowid_t const& rowid);
    
    virtual read_reult read(int32_t const& offset=0);
    virtual int32_t write(const void * data,int32_t bytes,int32_t const& offset=0);
    virtual int32_t bytes();
    ///@note need when multi-threading. in the multithead program, sqlite3_close_blob should be guarded by mutex, otherwise it will cause runtime error.
    virtual int32_t close();
    virtual void release();
    virtual ::sqlite3_blob* raw();
    virtual ::sqlite3* connection();
    
    
    
    
private:
    friend Blob* sqlite3_blob(::sqlite3 * database,const char schema[], const char table[],const char colname[]
        ,void * memman_object
        ,uint64_t (*memman_pos)(void * ptr_of_c11_string)
        ,void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data)
        ,void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos)                          
    
    ) noexcept;
    BlobInternal * m = nullptr;
};

[[nodiscard]]Blob* sqlite3_blob(::sqlite3 * database,const char schema[], const char table[],const char colname[]
    ,void * memman_object
    ,uint64_t (*memman_pos)(void * ptr_of_c11_string)
    ,void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data)
    ,void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos)
) noexcept;

} // sqlite3
} //namespace database{
} //namespace kautil{



#endif