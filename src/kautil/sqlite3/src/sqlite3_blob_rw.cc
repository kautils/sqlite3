
#include "../include/kautil/sqlite3/sqlite3.h"
#include "sqlite3.h"
#include <vector>
#include <string>
#include <iostream>
#include <cassert>


namespace kautil{
namespace database {

struct Col{
    std::string name;
    int type;
};




struct Sqlite3BlobInternal{

    ~Sqlite3BlobInternal(){
        if(ptr_ != nullptr) sqlite3_blob_close((sqlite3_blob*) ptr_);
        if(data_ != nullptr) std::free(data_);

    }

    sqlite3_blob* ptr_ = nullptr;
    void * data_ = nullptr;
    int use_size_ = 0;
    int size_ = 0;
    int blob_size_ = 0;
};



Sqlite3Blob::Sqlite3Blob(bool) {}
bool Sqlite3Blob::IsOpened() { return  m_ && m_->ptr_ != nullptr; }
Sqlite3Blob::Sqlite3Blob() : m_(new Sqlite3BlobInternal) {}
Sqlite3Blob::~Sqlite3Blob(){
    delete m_;
}


Sqlite3Blob::Sqlite3Blob(Sqlite3Blob && v){
    if(!v.m_) return ;
    delete m_;
    m_ = v.m_;
    v.m_ = nullptr;
}

Sqlite3Blob & Sqlite3Blob::operator=(Sqlite3Blob && v){
    if(!v.m_) return *this;
    delete m_;
    m_ = v.m_;
    v.m_ = nullptr;
    return *this;
}


void * Sqlite3Blob::ptr(){ return m_->ptr_; };
void * Sqlite3Blob::data(){ return m_->data_; }
std::size_t Sqlite3Blob::size(){ return m_->use_size_; }


Sqlite3Blob::Sqlite3Blob(void * ptr,int const& sizeof_blob) : m_(new Sqlite3BlobInternal){
    m_->ptr_ = reinterpret_cast<sqlite3_blob*>(ptr);
    m_->blob_size_=sizeof_blob;
}



struct Sqlite3BlobRWInternal{
    bool verbose_ = false;
    std::string table_type_;
    std::string qcol_;
    std::string table_name_;
    sqlite3 * database_;
    int last_result_ = 0;
};



Sqlite3BlobRW::Sqlite3BlobRW(void * sql, const char table[], bool temp)  : m_(new Sqlite3BlobRWInternal){
    m_->database_ = ((sqlite3*)sql);
    m_->table_type_ = (temp) ? "temp" : "main";
    m_->table_name_ = table;
}
Sqlite3BlobRW::~Sqlite3BlobRW(){ if(m_) delete m_; }

void Sqlite3BlobRW::err_msg(){
    std::cerr << sqlite3_errmsg(m_->database_) << std::endl;
}

int const &Sqlite3BlobRW::last_state() { return m_->last_result_; }


bool Sqlite3BlobRW::Read(Sqlite3Blob * rp_blob,int const& offset){
    if(offset >= rp_blob->m_->blob_size_){
        if(rp_blob->m_->data_) free(rp_blob->m_->data_);
        rp_blob->m_->use_size_=0;
        return false;
    }
    rp_blob->m_->use_size_ = rp_blob->m_->blob_size_ - offset;
    if(rp_blob->m_->use_size_ >= rp_blob->m_->size_ ){
        if(rp_blob->m_->data_) free(rp_blob->m_->data_);
        rp_blob->m_->size_ = rp_blob->m_->use_size_+1;  /* lack of +1 causes runtime error at sqlite3_blob_read. could not find essential reason of this. */
        rp_blob->m_->data_= (rp_blob->m_->use_size_) ? malloc(rp_blob->m_->size_) : nullptr;
    }
    return (m_->last_result_ = sqlite3_blob_read((sqlite3_blob*)rp_blob->m_->ptr_, rp_blob->m_->data_, (int) rp_blob->m_->use_size_, offset))==SQLITE_OK;
}


bool Sqlite3BlobRW::Write(Sqlite3Blob * rp_blob,int const& offset ){
    return (m_->last_result_ = sqlite3_blob_write((sqlite3_blob*)rp_blob->m_->ptr_, rp_blob->m_->data_, (int)rp_blob->m_->use_size_, offset)) == SQLITE_OK;
}

bool Sqlite3BlobRW::Write(Sqlite3Blob * rp_blob,const void * data,uint64_t const& size ,int const& offset){
    return (m_->last_result_ = sqlite3_blob_write((sqlite3_blob*)rp_blob->m_->ptr_, data, static_cast<int>(size), offset)) == SQLITE_OK;
}


bool Sqlite3BlobRW::ReOpen(Sqlite3Blob *blob_obj, const long long  &row) {
    if( (m_->last_result_ =  sqlite3_blob_reopen(reinterpret_cast<sqlite3_blob*>(blob_obj->m_->ptr_),row)) == SQLITE_ERROR){
        // https://www.sqlite.org/c3ref/blob_reopen.html
        /*
        The new row must meet the same criteria as for sqlite3_blob_open() -
        it must exist and there must be either a blob or text value stored in the nominated column.
        If the new row is not present in the table,
        or if it does not contain a blob or text value,
        or if another error occurs, an SQLite error code is returned
        and the blob handle is considered aborted. All subsequent calls to sqlite3_blob_read()
        , sqlite3_blob_write() or sqlite3_blob_reopen() on an aborted blob handle immediately
        return SQLITE_ABORT.
        Calling sqlite3_blob_bytes() on an aborted blob handle always returns zero.

        */
        return false;
    }
    blob_obj->m_->blob_size_ = sqlite3_blob_bytes(reinterpret_cast<sqlite3_blob*>(blob_obj->m_->ptr_));
    return true;
}

int Sqlite3BlobRW::LastResult(){
    return m_->last_result_;
}


Sqlite3Blob Sqlite3BlobRW::Open(int64_t const&row , char const col_name[], int flag  ){
    sqlite3_blob * ptr;
    if( (m_->last_result_ = sqlite3_blob_open(m_->database_,m_->table_type_.c_str(),m_->table_name_.c_str(),col_name,row,flag,&ptr)) == SQLITE_ERROR){
        return Sqlite3Blob(false);
    }


    return Sqlite3Blob{ptr, sqlite3_blob_bytes(ptr)};
}


//[[nodiscard]] Sqlite3Blob Sqlite3BlobRW::Open(int64_t const& row ,unsigned long long const& col,int flag ){ return Open(row,m_->cols_.at(col).name.c_str(),flag); }
//[[nodiscard]] Sqlite3Blob Sqlite3BlobRW::OpenRW(int64_t const& row ,unsigned long long const& col ){ return Open(row,col,1); }
[[nodiscard]] Sqlite3Blob Sqlite3BlobRW::OpenRW(int64_t const& row ,const char col[]){ return Open(row,col,1); }
//[[nodiscard]] Sqlite3Blob Sqlite3BlobRW::OpenR(int64_t const& row ,unsigned long long const& col ){  return Open(row,col,0); }
[[nodiscard]] Sqlite3Blob Sqlite3BlobRW::OpenR(int64_t const& row ,char const col[] ){  return Open(row,col,0); }




// todo : should be another module from the perspective of multi-thread >>>>>>>>>>>>>>>>>>>>>
//int Sqlite3BlobRW::UpdateCol(){
//    sqlite3_stmt * stmt;
//    int res;
//    if( (res = sqlite3_prepare_v2(m_->database_,("select * from " + m_->table_name_ + " limit 1").c_str(),-1,&stmt,nullptr)) != SQLITE_OK) return res;
//    if( (res = sqlite3_step(stmt)) < SQLITE_ROW) return res;
//    auto maxi = sqlite3_column_count(stmt);
//    for(auto i = 0; i < maxi; ++i){
//        m_->cols_.push_back({.name= sqlite3_column_name(stmt,i),.type= sqlite3_column_type(stmt,i)});
//    }
//    return sqlite3_finalize(stmt);
//}
//int Sqlite3BlobRW::Type(unsigned long const& i ){ return m_->cols_[i].type;  }
//const char * Sqlite3BlobRW::Name(unsigned long const& i ){ return m_->cols_[i].name.data();}
//int Sqlite3BlobRW::ColCount() {
//    return m_->cols_.size();
// todo : should be another module <<<<<<<<<<<<<<<<<<<<<<<<


void test_main_blob_object_memory_leak(){
    kautil::database::Sqlite3 db{":memory:"};
    db.SendQuery("create table if not exists t([rowid] integer primary key,[data] blob)");
    {
        auto stmt = db.Compile("insert or ignore into t(data) values(?)");
        for(auto i = 0; i < 100; ++i){
            db.SetBlob(&stmt,1,&i,sizeof(i));
            if(db.Step(&stmt,true) != SQLITE_DONE) db.error_msg();
        }
    }

    {
        using blob_obj_t = kautil::database::Sqlite3Blob;
        using blob_rw_t = kautil::database::Sqlite3BlobRW;

        auto b = blob_obj_t{};
        auto brw = blob_rw_t{db.database(),"t"};
        auto stmt = db.Compile("select rowid from t");
        for(;;){
            auto res = db.Step(&stmt,false);
            if(res != SQLITE_ROW)break;

            for(;;){
                static auto kCounter = int(0);
                auto rowid = sqlite3_column_int64(stmt.stmt(),0);
                b = brw.OpenR(rowid,"data");
                brw.Read(&b);
                if(++kCounter  % 10000==0){
                    int jjj = 0;
                }

            }
        }
    }

    std::cout << "A" << std::endl;

}




} //namespace database {
} //namespace kautil{









