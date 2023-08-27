#ifndef KAUTIL_SQLITE3_SRC_SERIALIZE_SERIALIZE_H
#define KAUTIL_SQLITE3_SRC_SERIALIZE_SERIALIZE_H

struct sqlite3;
typedef sqlite3 sqlite3;
#include <stdint.h>

namespace kautil{
namespace database {

struct SerializeInternal;
class Serialize{
public:
    Serialize();
    ~Serialize();
    int serialize(sqlite3 * db,const char * scheme);
    void * data();
    uint64_t size();
private:
    SerializeInternal * m =nullptr;
};

} //namespace database {
} //namespace kautil{

#endif