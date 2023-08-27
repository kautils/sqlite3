
#include <sqlite3.h>
#include <stdint.h>
#include <string>
#include "serialize.h"

namespace kautil{
namespace database {
namespace sqlite3{

struct SerializeInternal{

    SerializeInternal()=default;
    ~SerializeInternal(){ if(data) free(data); }

    int reset(){
        if(data){ free(data);data=0; }
        if(bytes)bytes = 0;
        if(db)db = 0;
        return 0;
    }

    ::sqlite3 * db=0;
    int64_t bytes=0;
    unsigned char * data=0;
    std::string schema;

};

void Serialize::release(){ delete this; }
Serialize::Serialize() : m(new SerializeInternal){}
Serialize::~Serialize(){ delete m; }
void * Serialize::data(){ return m->bytes ? reinterpret_cast<void*>(m->data) : nullptr; }
uint64_t Serialize::size(){ return m->bytes; }
bool Serialize::serialize(){
    m->data=sqlite3_serialize((m->db),(m->schema).data(),&m->bytes,0); // NOCOPY does not work well in my environment
    return m->data? 0:1;
}




Serialize* sqlite3_serialize(::sqlite3 * db,const char * schema) noexcept{
    auto res = new Serialize;
    res->m->db =db;
    res->m->schema = schema;
    return res;
}

} //namespace sqlite3 {
} //namespace database {
} //namespace kautil{




int tmain_kautil_sqlite3_serialize_static(sqlite3 * db,const char* schema){
    auto ser=kautil::database::sqlite3::sqlite3_serialize(db,schema);
    ser->serialize();
    auto bin = ser->data();
    auto bin_bytes=ser->size();
    ser->release();
    return 0;
}