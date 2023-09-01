


#include <stdint.h>

struct sqlite3;
typedef struct sqlite3 sqlite3; 

struct sqlite3_stmt;
typedef struct sqlite3_stmt sqlite3_stmt;


namespace kautil{
namespace database {



struct Sqlite3StmtInternal;
struct Sqlite3Stmt{
    
    using sqlite3_destructor_type = void (*)(void*); /*  0 : SQLITE_STATIC, 1: SQLITE_TRANSIENT . default is SQLITE_STATIC */
    
    virtual ~Sqlite3Stmt() noexcept;
    virtual ::sqlite3_stmt * raw() const noexcept;
    virtual int step(bool reset) const noexcept;
    virtual int reset() noexcept;
    virtual bool set_int(int const& i , int const& v )const noexcept;
    virtual bool set_double(int const& i , double const& v )const noexcept;
    virtual bool set_int64(int const& i , uint64_t const& v )const noexcept;
    
    
    virtual bool set_text(int const& i ,const char * data, uint64_t const& size ,sqlite3_destructor_type lifetime = 0  )const noexcept;
    virtual bool set_blob(int const& i ,const void *, uint64_t size,sqlite3_destructor_type lifetime = 0 )const noexcept;
    virtual bool set_null(int const& i ,const void *, uint64_t size,sqlite3_destructor_type lifetime = 0 )const noexcept;
    virtual bool set_zeroblob( const int &i, int)const noexcept;
    virtual void release()noexcept;
    
private:
    Sqlite3Stmt();
    Sqlite3StmtInternal * m = 0;
    friend Sqlite3Stmt * sqlite3_stmt(sqlite3 * dbcon,const char query[],bool * error, int nByte, const char **pzTail
        ,void * memman_object
        ,uint64_t (*memman_pos)(void * ptr_of_c11_string)
        ,void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data)
        ,void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos)) noexcept;

};



[[nodiscard]] Sqlite3Stmt * sqlite3_stmt(sqlite3 * dbcon,const char query[],bool * error, int nByte, const char **pzTail
    ,void * memman_object
    ,uint64_t (*memman_pos)(void * ptr_of_c11_string)
    ,void * (*memman_register)(void * ptr_of_c11_string,uint64_t pos,const void * data)
    ,void * (*memman_pointer)(void * ptr_of_c11_string,uint64_t pos)) noexcept;



} //namespace kautil{
} //namespace database {

