

#ifndef KAUTIL_SQLITE3_BLOB_H
#define KAUTIL_SQLITE3_BLOB_H

namespace temp_fix_miss{
namespace kautil{
namespace database{
namespace wsqlite3{

using sqlite_rowid_t = int64_t;
struct BlobInternal;
struct Blob{
    friend struct BlobInternal;
public :
    static int kRWFail;
    Blob();
    ~Blob();
    bool setup(sqlite3 * database,const char scheme[], const char table[],const char colname[]);
    bool is_opened();

    bool open_r(sqlite_rowid_t const& rowid);
    bool open_rw(sqlite_rowid_t const& rowid);
    bool reopen(sqlite_rowid_t const& rowid);
    int32_t read(void ** res,int32_t const& offset=0);
    int32_t write(const void * data,int32_t bytes,int32_t const& offset=0);
    int32_t bytes();

    ///@note need when multi-threading. in the multithead program, sqlite3_close_blob should be guarded by mutex, otherwise it will cause runtime error.
    int32_t close();

    
    const char * error_string();
private:
    BlobInternal * m = nullptr;
};




} // wsqlite3
} //namespace database{
} //namespace kautil{

} //namespace temp_fix_miss{



#endif