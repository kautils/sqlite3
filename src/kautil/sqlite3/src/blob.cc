


#include <stdint.h>
#include <string>
#include "kautil/sqlite3/sqlite3.h"
#include "sqlite3.h"
#include "cstring"
#include <vector>


namespace temp_fix_miss{
namespace kautil{
namespace database{


namespace wsqlite3{
static_assert(sizeof(sqlite_rowid_t)==sizeof(sqlite3_int64));

int Blob::kRWFail = -1;

using sqlite_rowid_t = int64_t;
struct BlobInternal{

    ~BlobInternal(){ close_if_opened(); }
    std::string col;
    std::string scheme;
    std::string table;
    sqlite3_blob * blobp=nullptr;
    sqlite3 * db= nullptr;
    std::vector<uint8_t> data;
    int whole_size=0; // int : sqlite3.h


    int close_if_opened(){
       if(blobp){ auto res=sqlite3_blob_close(blobp);blobp = nullptr; return res; }
        return SQLITE_OK;

    }

    bool open( sqlite_rowid_t const& rowid ,int flag){
        if(sqlite3_blob_open(db,scheme.data(),table.data(),col.data(),rowid,flag,&blobp)==SQLITE_ERROR){
            close_if_opened();
            if(blobp){ sqlite3_blob_close(blobp);blobp = nullptr; }
            whole_size =0;
            return false;
        }
        else{
            whole_size=sqlite3_blob_bytes(blobp);
            return true;
        }
    }
};



Blob::Blob(){}
Blob::~Blob(){ delete m; }
bool Blob::setup(sqlite3 * database,const char scheme[], const char table[],const char colname[]){
    if(!m) m = new BlobInternal;
//    m->db = reinterpret_cast<sqlite3*>(database);
    m->db = database;
    if(std::strcmp(scheme,"main") && std::strcmp(scheme,"temp")){
        // invalid scheme
        return false;
    }
    m->scheme =scheme;
    m->table = table;
    m->col= colname;
    return true;
}


bool Blob::is_opened(){ return m && m->blobp; }
bool Blob::open_r(sqlite_rowid_t const& rowid){ return m ?  m->open(rowid,0) : false; }
bool Blob::open_rw(sqlite_rowid_t const& rowid){ return m ? m->open(rowid,1) : false; }
bool Blob::reopen(sqlite_rowid_t const& rowid){
     //https://www.sqlite.org/c3ref/blob_reopen.html
    if(sqlite3_blob_reopen(reinterpret_cast<sqlite3_blob*>(m->blobp),rowid) == SQLITE_ERROR){
        m->close_if_opened();
        m->whole_size =0;
        return false;
    }
    m->whole_size = sqlite3_blob_bytes(reinterpret_cast<sqlite3_blob*>(m->blobp));
    return true;
}


int32_t Blob::read(void ** res,int32_t const& offset){
    if(!m->whole_size) return 0; // nothing to read
    if(offset > m->whole_size) return -1;
    m->data.resize(m->whole_size - offset);
    if( (SQLITE_OK==sqlite3_blob_read(m->blobp, m->data.data(),static_cast<int>(m->data.size()),offset)) ){
        if(res)*res=m->data.data();
        return static_cast<int32_t>(m->data.size());
    }else{
        return -1;
    }
    return static_cast<int32_t>(m->data.size());
}

int32_t Blob::write(const void * data,int32_t bytes,int32_t const& offset){
    return sqlite3_blob_write(m->blobp,data,bytes,offset) == SQLITE_OK ? 0:-1;
}


int32_t Blob::bytes(){ return m->whole_size; }
const char * Blob::error_string(){ return sqlite3_errmsg(m->db); }


int32_t Blob::close() { return m->close_if_opened(); }


} // wsqlite3
} //namespace database{
} //namespace kautil{

} //namespace temp_fix_miss{




//#define KAUTIL_SQLITE_BLOB_UNIT_TEST

#ifdef KAUTIL_SQLITE_BLOB_UNIT_TEST

int unit_test_blob_general_usage();
int main(){
    return unit_test_blob_general_usage();
}


#include <cassert>
int unit_test_blob_general_usage(){
    using namespace temp_fix_miss::kautil::database::wsqlite3;

    auto sql = kautil::database::Sqlite3{":memory:"};
    sql.SendQuery("create table if not exists t([rowid] integer primary key,[some] blob,[str] blob)");
    auto insert=sql.Compile("insert into t(some,str) values(?,?)");


    auto str = std::vector<const char *> {
        "aaaa"
        ,"bbbbbbbbbbbbbb"
        ,"ccc"
        ,"dddddddd"
        ,"eeeee"
    };

     for(auto i = 0; i < 100; ++i){
        sql.SetBlob(&insert,1,&i,sizeof(i));
        sql.SetBlob(&insert,2,str[i%str.size()],strlen(str[i%str.size()]));
        assert(sql.Step(&insert,true)==SQLITE_DONE);
    }

    auto step_all = [&](const char col[],int(*cb)(Blob & blob,rowid_t const& rowid)){ // read only
        auto stmt = sql.Compile("select rowid from t");
        Blob blob;
        assert(blob.setup(sql.database(),"main","t",col));
        for(;;){
            auto res = sql.Step(&stmt,false);
            if(res != SQLITE_ROW) break;
            auto rowid = sqlite3_column_int64(to_stmt(stmt),0);
            if(0!=cb(blob,sqlite3_column_int64(to_stmt(stmt),0)))return 1;
        }
        return 0;
    };


    auto show_some = [](auto blob,auto rowid)->int{ // read only
        auto size=int(0);
        auto col=(int *)0;
        if(  (!blob.is_opened() ? !blob.open_r(rowid) :!blob.reopen(rowid)) ){
            printf("%s\n",blob.error_string());
            fflush(stdout);
            return 1;
        }
        Blob::kRWFail == (size=blob.read((void**)&col)) ?
              printf("%s\n", blob.error_string())
            : printf("%d (%d bytes)\n", *col,size);
        return 0;
    };
    step_all("some",show_some);

    step_all("some",[](auto blob,auto rowid)->int{ // write and read
        auto val_bytes=(0);
        auto val=(int32_t*)0;
        if( (!blob.is_opened() ? !blob.open_rw(rowid) :!blob.reopen(rowid)) ){
            printf("%s\n",blob.error_string());
            return 1;
        }
        if( Blob::kRWFail== (val_bytes=blob.read((void**)&val)) ){
            printf("%s\n",blob.error_string());
            return 1;
        }
        if( Blob::kRWFail== blob.write(&(*val=*val/2),sizeof(*val))){
            printf("%s\n",blob.error_string());
            return 1;
        }
        return 0;
    });
    step_all("some",show_some);


    {// column bytes
        step_all("str",[](auto blob,auto rowid)->int{
            auto str = (char*)0;
            auto str_bytes=int(0);
            if( (!blob.is_opened() ? !blob.open_rw(rowid) :!blob.reopen(rowid)) ){
                printf("%s\n",blob.error_string());
                return 1;
            }
            printf("%d\n",blob.bytes());
            return 0;
        });
    }
    return 0;
}


#endif