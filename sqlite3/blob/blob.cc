


#include <stdint.h>
#include <string>
#include "sqlite3.h"
#include "cstring"
#include <vector>
#include "blob.h"


namespace kautil{
namespace database{
namespace sqlite3{



static_assert(sizeof(sqlite_rowid_t)==sizeof(sqlite3_int64));


using sqlite_rowid_t = int64_t;
struct BlobInternal{

    ~BlobInternal(){ close_if_opened(); }
    std::string col;
    std::string schema;
    std::string table;
//    ::sqlite3_blob * blobp=nullptr;
    ::sqlite3 * db= nullptr;
    std::vector<uint8_t> data;
    int whole_size=0; // int : sqlite3.h
    
    uint64_t pos;
    void * memman_object = 0;
    uint64_t (*memman_pos)(void * ptr_of_c11_string) = 0;
    void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data) = 0;
    void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos) = 0;
    

    ::sqlite3_blob * raw(){
        return reinterpret_cast<::sqlite3_blob*>(memman_pointer(memman_object,pos));
    }
    
    
    int close_if_opened(){
       if(auto blob=raw()){ auto res=sqlite3_blob_close(blob); memman_register(memman_object,pos, nullptr); return res; }
        return SQLITE_OK;
    }

    bool open( sqlite_rowid_t const& rowid ,int flag){
        auto blobp = (::sqlite3_blob*)0;
        if(sqlite3_blob_open(db,schema.data(),table.data(),col.data(),rowid,flag,&blobp)==SQLITE_ERROR){
            close_if_opened();
            whole_size =0;
            return false;
        }
        else{
            whole_size=sqlite3_blob_bytes(blobp);
            pos =memman_pos(memman_object);
            memman_register(memman_object,pos,blobp);
            memman_register(memman_object,pos,blobp);
            return true;
        }
    }
};



Blob::Blob(){}
Blob::~Blob(){ delete m; }
bool Blob::setup(::sqlite3 * database,const char schema[], const char table[],const char colname[]){
    if(!m) m = new BlobInternal;
    m->db = database;
    if(std::strcmp(schema,"main") && std::strcmp(schema,"temp")){
        // invalid schema
        return false;
    }
    m->schema =schema;
    m->table = table;
    m->col= colname;
    return true;
}


void Blob::release(){ 
    delete this; 
}
::sqlite3_blob* Blob::raw(){ return m->raw(); }
::sqlite3* Blob::connection(){ return m->db; }


bool Blob::is_opened(){ return m && raw(); }
bool Blob::open_r(sqlite_rowid_t const& rowid){ return m ?  m->open(rowid,0) : false; }
bool Blob::open_rw(sqlite_rowid_t const& rowid){ return m ? m->open(rowid,1) : false; }
bool Blob::reopen(sqlite_rowid_t const& rowid){
     //https://www.sqlite.org/c3ref/blob_reopen.html
    if(sqlite3_blob_reopen(reinterpret_cast<::sqlite3_blob*>(raw()),rowid) == SQLITE_ERROR){
        m->close_if_opened();
        m->whole_size =0;
        return false;
    }
    m->whole_size = sqlite3_blob_bytes(reinterpret_cast<::sqlite3_blob*>(raw()));
    return true;
}


Blob::read_reult Blob::read(int32_t const& offset){
    if(!m->whole_size) return {}; // nothing to read
    if(offset > m->whole_size) return {};
    m->data.resize(m->whole_size - offset);
    auto res = read_reult{};
    if( (SQLITE_OK==sqlite3_blob_read(raw(), m->data.data(),static_cast<int>(m->data.size()),offset)) )return read_reult{.data=m->data.data(),.size=static_cast<int32_t>(m->data.size())};
    else return read_reult{.size=-1};
}

int32_t Blob::write(const void * data,int32_t bytes,int32_t const& offset){
    return sqlite3_blob_write(raw(),data,bytes,offset) == SQLITE_OK ? 0:-1;
}


int32_t Blob::bytes(){ return m->whole_size; }
int32_t Blob::close() { return m->close_if_opened(); }

Blob *sqlite3_blob(::sqlite3 *dbcon, const char *schema, const char *table, const char *colname
        ,void * memman_object
        ,uint64_t (*memman_pos)(void * ptr_of_c11_string)
        ,void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data)
        ,void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos)
) noexcept {
    auto res =new Blob;
    if(res->setup(dbcon,schema,table,colname)){
        res->m->memman_object=memman_object;
        res->m->memman_pos=memman_pos;
        res->m->memman_pointer=memman_pointer;
        res->m->memman_register=memman_register;
        return res;
    }else{
        delete res;
        return nullptr;
    }
}


} // wsqlite3
} //namespace database{
} //namespace kautil{


int tmain_kautil_sqlite3_blob_static(sqlite3 * con
    ,void * memman_object
    ,uint64_t (*memman_pos)(void * ptr_of_c11_string)
    ,void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data)
    ,void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos)
) {
//    while(true){
//        for(auto i = 0; i< 10000; ++i) {
        auto b= kautil::database::sqlite3::sqlite3_blob(con,"main","test","data",memman_object,memman_pos,memman_register,memman_pointer);
        auto id = 1;
        if(b->is_opened() ? !b->reopen(id) : !b->open_r(id)){
            fprintf(stderr,"%s\n",sqlite3_errmsg(con));
            fflush(stderr);
            throw false;
        } 
        auto [d,s] = b->read();
        if(s < 0){
            fprintf(stderr,"err\n");
            fflush(stderr);
        }else{
            printf("%.*s\n",s,d);
            fflush(stdout);
        }
        b->release();
//        }
//    }
    return 0;
}
