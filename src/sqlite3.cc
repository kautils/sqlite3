#include "../include/kautil/sqlite3/sqlite3.h"
#include <cassert>
#include <iostream>
#include "sqlite3.h"



namespace kautil{
namespace database {
namespace sqlite {

sqlite3_destructor_type Sqlite3::kSQLITE_STATIC = SQLITE_STATIC;
sqlite3_destructor_type Sqlite3::kSQLITE_TRANSIENT = SQLITE_TRANSIENT;


Sqlite3AnyBase::Sqlite3AnyBase(Type type, void *entity) : type_(type), rp_entity_(entity) {};

//Sqlite3AnyBase::Sqlite3AnyBase(Type type) : type_(type) {};
Sqlite3AnyBase::Sqlite3AnyBase(Type type, void *entity, size_t size) : type_(type), rp_entity_(entity), size_(size) {};

Sqlite3AnyBase::Sqlite3AnyBase(Type type, void *entity, size_t size, sqlite3_destructor_type lifetime) : type_(type),
                                                                                                         rp_entity_(
                                                                                                                 entity),
                                                                                                         size_(size),
                                                                                                         lifetime_(
                                                                                                                 lifetime) {};

Sqlite3AnyBase::Sqlite3AnyBase(Type type, void *entity, size_t size, sqlite3_destructor_type lifetime, uint8_t encoding)
        : type_(type), rp_entity_(entity), size_(size), lifetime_(lifetime), encoding_(encoding) {};


Sqlite3AnyBase::Sqlite3AnyBase(const std::string &val) : type_(kText), rp_entity_((void *) val.data()),
                                                         size_(val.size()) {}

Sqlite3AnyBase::Sqlite3AnyBase(const double &val) : type_(kDouble), rp_entity_((void *) &val) {}

Sqlite3AnyBase::Sqlite3AnyBase(const int &val) : type_(kInt), rp_entity_((void *) &val) {}

Sqlite3AnyBase::Sqlite3AnyBase(const int64_t &val) : type_(kInt64), rp_entity_((void *) &val) {}
//Sqlite3AnyBase::Sqlite3AnyBase(void*const&val) : type_(kBlob),rp_entity_((void*)&val) {}


Sqlite3AnyBase::operator char *() const { return (char *) rp_entity_; }

Sqlite3AnyBase::operator void *() const { return (void *) rp_entity_; }

Sqlite3AnyBase::operator double() const { return *((double *) rp_entity_); }

Sqlite3AnyBase::operator int() const { return *((int *) rp_entity_); }

Sqlite3AnyBase::operator int64_t() const {
    return *((int64_t *) rp_entity_);
}

Sqlite3AnyBase::operator uint64_t() const { return *((uint64_t *) rp_entity_); }

Sqlite3AnyBase::operator sqlite3_value *() const { return (sqlite3_value *) rp_entity_; }


//Sqlite3AnyBase::LifetimeT Sqlite3AnyBase::lifetime() const { return lifetime_; }
sqlite3_destructor_type Sqlite3AnyBase::lifetime() const { return lifetime_; }

void *Sqlite3AnyBase::rp_entity() const { return rp_entity_; }

int Sqlite3AnyBase::size() const { return size_; }

Sqlite3AnyBase::Type Sqlite3AnyBase::type() const { return type_; }

char *Sqlite3AnyBase::sqlite3_bind_pointer_4th() const { return sqlite3_bind_pointer_4th_; }

unsigned char Sqlite3AnyBase::encoding() const { return encoding_; }


Sqlite3Any::Sqlite3Any(const std::string &val) : Sqlite3AnyBase(val) {}

Sqlite3Any::Sqlite3Any(const double &val) : Sqlite3AnyBase(val) {}

Sqlite3Any::Sqlite3Any(const int64_t &val) : Sqlite3AnyBase(val) {}

Sqlite3Any::Sqlite3Any(const int &val) : Sqlite3AnyBase(val) {}
//Sqlite3Any::Sqlite3Any(void*const&val) : Sqlite3AnyBase(val) {}



Int64::Int64(int64_t const &val) : Sqlite3AnyBase(Sqlite3AnyBase::kInt64, (void *) &val) {}

Int::Int(int const &val) : Sqlite3AnyBase(Sqlite3AnyBase::kInt, (void *) &val) {}

Blob::Blob(void *val, size_t size, void(*lifetime_)(void *)) : Sqlite3AnyBase(Type::kBlob, val, size, lifetime_) {}

Blob::Blob(std::string const &bin_str, void(*lifetime_)(void *)) : Sqlite3AnyBase(Type::kBlob, (void *) bin_str.data(),
                                                                                  bin_str.size(), lifetime_) {}

Blob::Blob(std::vector<uint8_t> const &bin_v, void(*lifetime_)(void *)) : Sqlite3AnyBase(Type::kBlob,
                                                                                         (void *) bin_v.data(),
                                                                                         bin_v.size(), lifetime_) {}

Blob64::Blob64(void *val, size_t size, void(*lifetime_)(void *) /*= SQLITE_STATIC*/) : Sqlite3AnyBase(
        Sqlite3AnyBase::kBlob64, val, size, lifetime_) {}

Text::Text(const char *val, size_t size, void(*lifetime_)(void *) /*= SQLITE_STATIC*/) : Sqlite3AnyBase(
        Sqlite3AnyBase::kText, (void *) &val, size, lifetime_) {}

Text::Text(std::string const& val,void(*lifetime_)(void *) /*= SQLITE_STATIC*/) : Sqlite3AnyBase(
    Sqlite3AnyBase::kText, (void *) val.data(), val.size(), lifetime_) {}
        
Double::Double(double const &val) : Sqlite3AnyBase(Sqlite3AnyBase::kDouble, (void *) &val) {}

Null::Null() : Sqlite3AnyBase(Sqlite3AnyBase::kNull, nullptr) {}

Value::Value(const sqlite3_value *val) : Sqlite3AnyBase(Sqlite3AnyBase::kValue, (void *) &val) {}

Text16::Text16(void *val, int size, sqlite3_destructor_type lifetime) : Sqlite3AnyBase(Sqlite3AnyBase::kText16,
                                                                                       (void *) &val, size, lifetime) {}

Text64::Text64(const char *val, int size, sqlite3_destructor_type lifetime, uint8_t encoding) : Sqlite3AnyBase(
        Sqlite3AnyBase::kText64, (void *) &val, size, lifetime, encoding) {}

ZeroBlob::ZeroBlob(int const &val) : Sqlite3AnyBase(Sqlite3AnyBase::kZeroBlob, (void *) &val) {}

ZeroBlob64::ZeroBlob64(uint64_t const &val) : Sqlite3AnyBase(Sqlite3AnyBase::kZeroBlob64, (void *) &val) {}
//Pointer::Pointer(void*, const char*,void(*)(void*)) : Sqlite3AnyBase(Sqlite3AnyBase::kPointer){}



Sqlite3Any::Sqlite3Any(const Sqlite3AnyBase &val) : Sqlite3AnyBase(val) {

}

int DefaultCallBack(void *custom_parameter, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s( %s ), ", azColName[i], argv[i] ? argv[i] : "NULL");
//        printf(" %s , ",/* azColName[i],*/ argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}





Sqlite3::Sqlite3(std::filesystem::path const &dbpath,int flags,std::string const& zvfs){
    if(sqlite3_open_v2(
            (char *) dbpath.c_str()
            ,&database_
            ,flags
            , nullptr) != SQLITE_OK ){
        std::cout << sqlite3_errmsg(database_) << std::endl;
        assert(false);
    }
}

Sqlite3::Sqlite3(sqlite3 *db) {
    database_ = db;
}

Sqlite3::Sqlite3(std::filesystem::path const &dbpath) {
    if (sqlite3_open((char *) dbpath.c_str(), &database_)) {
        perror("perror");
        assert(false);
    }
};

Sqlite3::~Sqlite3() { if (database_) sqlite3_close(database_); };

sqlite3 *Sqlite3::database() { return database_; }

sqlite3_stmt *Sqlite3::stmt() { return stmt_; }


std::unique_ptr<std::vector<uint8_t>>
Sqlite3::ReadBlob(std::string table_name, std::string const &col_name, int64_t row_id, DataBaseMode db_mode) {
    auto up_buffer = std::make_unique<std::vector<uint8_t>>();
    sqlite3_blob *blob;
    // https://www.sqlite.org/c3ref/blob_open.html
    if (
            sqlite3_blob_open(
                    database_,
                    (db_mode == 0) ? "main" : "temp",  // database symbol
                    "some",  // table
                    col_name.data()  // column
                    , row_id // id, seems to start with 1
                    , 0 // flag (0 : RDONLY | !0 RDWR)
                    , &blob // res
            ) != SQLITE_OK) {
        return nullptr;
    }
    up_buffer->resize(sqlite3_blob_bytes(blob));
    sqlite3_blob_read(blob, up_buffer->data(), up_buffer->size(), 0);
    return std::move(up_buffer);
}


uint64_t Sqlite3::Count(std::string const &table_name) {
    uint64_t res;
    SendQuery("select Count(*) from " + table_name, &res, [](auto row_count, auto argc, auto argv, auto) {
        *((uint64_t *) row_count) = argc ? std::atoll(argv[0]) : 0;
        return SQLITE_OK;
    });
    return res;

}


bool Sqlite3::SendQuery(std::string const &query, void *parameter, decltype(DefaultCallBack) fp_callback) {
    char *tail;
    auto rc = sqlite3_exec(database_, query.data(), fp_callback, parameter, &tail);
    if (auto success = (rc != SQLITE_OK)) {
        std::cerr << tail;
        sqlite3_free(tail);
        return success;
    } else {
        return success;
    }
};


//template<typename CallBackFuncT >
bool
Sqlite3::SendQueryWithFile(std::filesystem::path const &path, void *parameter, decltype(DefaultCallBack) fp_callback) {
    using std::chrono::system_clock;
    auto st = system_clock::now();
    namespace fs = std::filesystem;
    std::string query;
    auto file = fs::path(path);
    query.resize(fs::file_size(file));
    std::ifstream(file, std::ios::binary | std::ios::in).read(query.data(), query.size());
    SendQuery(query, parameter, fp_callback);


    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
    std::cout << query << "\n";
    std::cout << (double) (system_clock::now() - st).count() / system_clock::period::den << " (sec)" << "\n\n";
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;

    return true;

}


bool Sqlite3::Bind(const std::string &query, const std::vector<Sqlite3Any> &binds) {

    if (sqlite3_prepare_v2(database_, query.data(), -1, &stmt_, NULL) != SQLITE_OK)
        std::cerr << "prepare failed: "
                  << sqlite3_errmsg(
                          database_)
                  << std::endl;
    int index = 1;
    for (auto const &bind: binds) if (!AutoSwitch(index++, bind))std::cerr << sqlite3_errmsg(database_) << std::endl;
    if (sqlite3_step(stmt_) != SQLITE_DONE) std::cerr << "execution failed: " << sqlite3_errmsg(database_) << std::endl;
    sqlite3_finalize(stmt_);
    return true;
}


std::string Sqlite3::PathToDatabase() {
    auto res = sqlite3_db_filename(database_, "main");
    return (res) ? res : ((res = sqlite3_db_filename(database_, "temp")) ? res : "");
}


/**
 @note : refer from sqlite3.h
    SQLITE_API int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
    SQLITE_API int sqlite3_bind_blob64(sqlite3_stmt*, int, const void*, sqlite3_uint64, void(*)(void*));
    SQLITE_API int sqlite3_bind_double(sqlite3_stmt*, int, double);
    SQLITE_API int sqlite3_bind_int(sqlite3_stmt*, int, int);
    SQLITE_API int sqlite3_bind_int64(sqlite3_stmt*, int, sqlite3_int64);
    SQLITE_API int sqlite3_bind_null(sqlite3_stmt*, int);
    SQLITE_API int sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*));
    SQLITE_API int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int, void(*)(void*));
    SQLITE_API int sqlite3_bind_text64(sqlite3_stmt*, int, const char*, sqlite3_uint64, void(*)(void*), unsigned char encoding);
    SQLITE_API int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
    SQLITE_API int sqlite3_bind_pointer(sqlite3_stmt*, int, void*, const char*,void(*)(void*));
    SQLITE_API int sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);
    SQLITE_API int sqlite3_bind_zeroblob64(sqlite3_stmt*, int, sqlite3_uint64);
*/


bool Sqlite3::AutoSwitch(int index, const Sqlite3Any &arg) {

    switch (arg.type()) {
        case Sqlite3AnyBase::kBlob       :
            return sqlite3_bind_blob(stmt_, index, arg, arg.size(), SQLITE_STATIC) == SQLITE_OK;
        case Sqlite3AnyBase::kBlob64     :
            return sqlite3_bind_blob64(stmt_, index, arg, arg.size(), SQLITE_STATIC) == SQLITE_OK;
        case Sqlite3AnyBase::kInt        :
            return sqlite3_bind_int(stmt_, index, arg) == SQLITE_OK;
        case Sqlite3AnyBase::kInt64      :
            return sqlite3_bind_int64(stmt_, index, (int64_t) arg) == SQLITE_OK;
        case Sqlite3AnyBase::kDouble     :
            return sqlite3_bind_double(stmt_, index, (double) arg) == SQLITE_OK;
        case Sqlite3AnyBase::kNull       :
            return sqlite3_bind_null(stmt_, index) == SQLITE_OK;
        case Sqlite3AnyBase::kText       :
            return sqlite3_bind_text(stmt_, index, arg, arg.size(), arg.lifetime()) == SQLITE_OK;
        case Sqlite3AnyBase::kText16     :
            return sqlite3_bind_text16(stmt_, index, arg, arg.size(), arg.lifetime()) == SQLITE_OK;
        case Sqlite3AnyBase::kText64     :
            return sqlite3_bind_text64(stmt_, index, arg, arg.size(), arg.lifetime(), arg.encoding()) == SQLITE_OK;
        case Sqlite3AnyBase::kZeroBlob   :
            return sqlite3_bind_zeroblob(stmt_, index, arg) == SQLITE_OK;
        case Sqlite3AnyBase::kZeroBlob64 :
            return sqlite3_bind_zeroblob(stmt_, index, (uint64_t) arg) == SQLITE_OK;
        case Sqlite3AnyBase::kValue      :
            return sqlite3_bind_value(stmt_, index, arg) == SQLITE_OK;
        case Sqlite3AnyBase::kPointer    :
            return sqlite3_bind_pointer(stmt_, index, arg, arg.sqlite3_bind_pointer_4th(), arg.lifetime()) == SQLITE_OK;
        default:
            std::cerr << "not implemented yet or invalid value was passed." << std::endl;
            std::cerr << "err number is \"" << static_cast<uint16_t>(arg.type()) << "\"" << std::endl;
            assert(false);
    };

    return false;
}

constexpr char *kQueryBeginTransaction = (char *) "begin transaction;";
constexpr char *kQueryCommit = (char *) "commit;";

bool Sqlite3::BeginTransaction() { return SendQuery(kQueryBeginTransaction); }

bool Sqlite3::EndTransaction() { return SendQuery(kQueryCommit); }

bool Sqlite3::Commit() { return SendQuery(kQueryCommit); }


} 
} //namespace sqlite3 {
} //namespace kautil{



