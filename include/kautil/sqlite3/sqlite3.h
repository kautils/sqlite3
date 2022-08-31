

#ifndef _MNT_NVME0N1_GIT_LOCAL_2_LOCAL_DAEMON_FIFO_TIPS_DIRTY_SQLITE_GEN_GEN_H
#define _MNT_NVME0N1_GIT_LOCAL_2_LOCAL_DAEMON_FIFO_TIPS_DIRTY_SQLITE_GEN_GEN_H


#include <filesystem>
#include <fstream>
#include <vector>


// forward declaration
struct sqlite3;
struct sqlite3_stmt;
struct sqlite3_value;
typedef void (*sqlite3_destructor_type)(void*);
    //#define SQLITE_STATIC      ((sqlite3_destructor_type)0)
    //#define SQLITE_TRANSIENT   ((sqlite3_destructor_type)-1)

#define __SQLITE3_ERROR(database,code,program) \
if(program != code) std::cerr << sqlite3_errmsg(database) << std::endl;



namespace kautil{
namespace database{
namespace sqlite {

class Sqlite3AnyBase {

public:

    enum Type : uint16_t {
        kBlob, kBlob64, kZeroBlob, kZeroBlob64,
        kDouble,
        kInt, kInt64,
        kNull,
        kText, kText16, kText64,
        kValue,
        kPointer
    };

    Sqlite3AnyBase(Sqlite3AnyBase const &) = default;

    virtual ~Sqlite3AnyBase() = default;

protected:

//    using LifetimeT = sqlite3_destructor_type /*void(*)(void*)*/;

    Sqlite3AnyBase(Type type, void *entity);

    Sqlite3AnyBase(Type type, void *entity, size_t size);

    Sqlite3AnyBase(Type type, void *entity, size_t size, void(*lifetime)(void *));

    Sqlite3AnyBase(Type type, void *entity, size_t size, void(*lifetime)(void *), uint8_t encoding);


    // support only text ,real , integer (bindings of string, double, int, int64)
    Sqlite3AnyBase(std::string const &);

    Sqlite3AnyBase(double const &);

    Sqlite3AnyBase(int const &);

    Sqlite3AnyBase(int64_t const &);
//    Sqlite3AnyBase(void* const&); // // for blobs, need size of binary, so have to go through object. same for text



    operator char *() const;

    operator void *() const;

    operator double() const;

    operator int() const;

    operator int64_t() const;

    operator uint64_t() const; // sqlite3_uint64
    operator sqlite3_value *() const;


    void *rp_entity() const;

    int size() const;

    Type type() const;

    sqlite3_destructor_type lifetime() const;

    unsigned char encoding() const;

    char *sqlite3_bind_pointer_4th() const;

private :
    Type type_;
    void *rp_entity_;
    int size_;

    void (*lifetime_)(void *);

    unsigned char encoding_;
    char *sqlite3_bind_pointer_4th_;

}; // Sqlite3AnyBase




class Sqlite3Any : private Sqlite3AnyBase {

public:
    
//    using Sqlite3AnyBase::LifetimeT;

    Sqlite3Any(std::string const &val);

    Sqlite3Any(double const &val);

    Sqlite3Any(int64_t const &val);

    Sqlite3Any(int const &val);
//    Sqlite3Any(void *const& val);

    Sqlite3Any(Sqlite3AnyBase const &);

    ~Sqlite3Any() = default;

    using Sqlite3AnyBase::type;
    using Sqlite3AnyBase::rp_entity;
    using Sqlite3AnyBase::size;
    using Sqlite3AnyBase::lifetime;
    using Sqlite3AnyBase::encoding;
    using Sqlite3AnyBase::sqlite3_bind_pointer_4th;
    using Sqlite3AnyBase::operator char *;
    using Sqlite3AnyBase::operator void *;
    using Sqlite3AnyBase::operator double;
    using Sqlite3AnyBase::operator int;
    using Sqlite3AnyBase::operator int64_t;
    using Sqlite3AnyBase::operator uint64_t; // sqlite3_uint64
    using Sqlite3AnyBase::operator sqlite3_value *;

}; // class Sqlite3Any




/*static*/ int DefaultCallBack(void *NotUsed, int argc, char **argv, char **azColName);

class Sqlite3 {

    sqlite3 *database_ = nullptr;
    sqlite3_stmt *stmt_ = nullptr;


    enum DataBaseMode : uint32_t {
        kMain = 0, kTEMP = 1
    };

public:

    Sqlite3(sqlite3 *db);

    Sqlite3(std::filesystem::path const &dbpath);
    
    // sqlite3_open_v2
    Sqlite3(std::filesystem::path const &dbpath,int flags,std::string const& zvfs = "");
    
    ~Sqlite3();


    sqlite3 *database();

    sqlite3_stmt *stmt();

    bool SendQuery(std::string const &query, void *parameter = nullptr,
                   decltype(DefaultCallBack) fp_callback = DefaultCallBack);

    bool Bind(std::string const &query, std::vector<Sqlite3Any> const &binds);

    bool SendQueryWithFile(std::filesystem::path const &path, void *parameter = nullptr,
                           decltype(DefaultCallBack) fp_callback = DefaultCallBack);

    bool BeginTransaction();

    bool EndTransaction(); /// @note equal to Commit
    bool Commit();/// @note equal to EndTransaction


    static sqlite3_destructor_type kSQLITE_STATIC;
    static sqlite3_destructor_type kSQLITE_TRANSIENT;

    /// @return binary data in the column , if fail return nullptr
    /// @note row_id is not index, starting from  1  
    std::unique_ptr<std::vector<uint8_t>>
    ReadBlob(std::string table_name, std::string const &col_name, int64_t row_id, DataBaseMode db_mode = kMain);


    /// @return the total number of rows in "table_name" 
    uint64_t Count(std::string const &table_name);

    std::string PathToDatabase();


private:

    bool AutoSwitch(int index, Sqlite3Any const &arg);


}; // class



class Blob : public Sqlite3AnyBase {
public :
    Blob(void *, size_t size, sqlite3_destructor_type lifetime = Sqlite3::kSQLITE_STATIC);

    virtual ~Blob() = default;

    Blob(std::string const &bin_str, sqlite3_destructor_type lifetime = Sqlite3::kSQLITE_STATIC);

    Blob(std::vector<uint8_t> const &bin_str, sqlite3_destructor_type lifetime = Sqlite3::kSQLITE_STATIC);
};

class Blob64 : public Sqlite3AnyBase {
public :
    Blob64(void *, size_t size, sqlite3_destructor_type lifetime);

    virtual ~Blob64() = default;
};

class ZeroBlob : private Sqlite3AnyBase {
public :
    ZeroBlob(int const &);

    virtual ~ZeroBlob() = default;
};

class ZeroBlob64 : private Sqlite3AnyBase {
public :
    ZeroBlob64(uint64_t const &);

    virtual ~ZeroBlob64() = default;
};

class Text : public Sqlite3AnyBase {
public :
    Text(const char *, size_t size, sqlite3_destructor_type lifetime = Sqlite3::kSQLITE_STATIC);
    Text(std::string const& val, sqlite3_destructor_type lifetime = Sqlite3::kSQLITE_STATIC);

    virtual ~Text() = default;
};

class Text16 : private Sqlite3AnyBase {
public :
    Text16(void *, int size, sqlite3_destructor_type lifetime);

    virtual ~Text16() = default;
};

class Text64 : private Sqlite3AnyBase {
public :
    Text64(const char *, int size, sqlite3_destructor_type lifetime, uint8_t encoding);

    virtual ~Text64() = default;
};


class Null : public Sqlite3AnyBase {
public :
    Null();

    virtual ~Null() = default;
};

class Value : public Sqlite3AnyBase {
public :
    Value(const sqlite3_value *);

    virtual ~Value() = default;
};

class Double : public Sqlite3AnyBase {
public :
    Double(double const &);

    virtual ~Double() = default;
};

class Int : public Sqlite3AnyBase {
public :
    Int(int const &);

    virtual ~Int() = default;
};

class Int64 : public Sqlite3AnyBase {
public :
    Int64(int64_t const &);

    virtual ~Int64() = default;
};

class Pointer : private Sqlite3AnyBase {
public :
    Pointer(void *, const char *, sqlite3_destructor_type);

    virtual ~Pointer() = default;
};

}
} //namespace sqlite3{
} //namespace kautil{


#endif
