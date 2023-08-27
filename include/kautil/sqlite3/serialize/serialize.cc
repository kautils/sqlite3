
#include <sqlite3.h>
#include <stdint.h>
#include <string>
#include "serialize.h"

namespace kautil{
namespace database {

struct SerializeInternal{

    SerializeInternal()=default;
    ~SerializeInternal(){ if(data) free(data); }

    int reset(){
        if(data){ free(data);data=0; }
        if(bytes)bytes = 0;
        if(db)db = 0;
        return 0;
    }

    int serialize(sqlite3 * sql,const char * schm){
        reset();
        data=sqlite3_serialize((db = sql),(scheme=schm).data(),&bytes,0); // NOCOPY does not work well in my environment
        return data? 0:1;
    }

    sqlite3 * db=0;
    int64_t bytes=0;
    unsigned char * data=0;
    std::string scheme;

};


Serialize::Serialize() : m(new SerializeInternal){}
Serialize::~Serialize(){ delete m; }
int Serialize::serialize(sqlite3 * db,const char * scheme){ return m->serialize(db,scheme); }
void * Serialize::data(){ return m->bytes ? reinterpret_cast<void*>(m->data) : nullptr; }
uint64_t Serialize::size(){ return m->bytes; }


} //namespace database {
} //namespace kautil{


int tmain_kautil_sqlite_serialize(sqlite3 * db,const char* scheme){
    {
        auto ser=kautil::database::Serialize{};
        ser.serialize(db,scheme);
        auto bin = ser.data();
        auto bin_bytes=ser.size();
    }
    return 0;
}