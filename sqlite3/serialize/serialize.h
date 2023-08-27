#ifndef KAUTIL_SQLITE3_SRC_SERIALIZE_SERIALIZE_H
#define KAUTIL_SQLITE3_SRC_SERIALIZE_SERIALIZE_H

#include <stdint.h>

struct sqlite3;
typedef sqlite3 sqlite3;

namespace kautil{
namespace database {
namespace sqlite3{

struct SerializeInternal;
class Serialize{
public:
    Serialize();
    virtual ~Serialize();
    virtual void * data();
    virtual uint64_t size();
    virtual bool serialize();
    virtual void release();
    friend Serialize* sqlite3_serialize(::sqlite3 * db,const char * schema) noexcept;
private:
    SerializeInternal * m =nullptr;
};


Serialize* sqlite3_serialize(::sqlite3 * db,const char * schema) noexcept;



} //namespace sqlite3 {
} //namespace database {
} //namespace kautil{

#endif