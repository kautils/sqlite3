

#include "extern.h"

#if defined(WIN32) || defined(WIN64) || defined(_MSC_VER)

#include <libloaderapi.h>

int rtld_lazy = 0;
int rtld_now = 1;
int rtld_global = 2;
int rtld_local = 4;
int rtld_nodelete = 8;
int rtld_noload = 16;
int rtld_deepbind = 32;

void* kautil_dlopen(const char * path_to_shared_lib, int flags) { return LoadLibraryA((char*)path_to_shared_lib); }
void* kautil_dlsym(void * handle, const char *symbol) { return (void *) GetProcAddress(reinterpret_cast<HMODULE>(handle),(char *)symbol); }
int kautil_dlclose(void * handle) { return FreeLibrary(reinterpret_cast<HMODULE>(handle));}
#else

#include <dlfcn.h>
int rtld_lazy = RTLD_LAZY;
int rtld_now = RTLD_NOW;
int rtld_global = RTLD_GLOBAL;
int rtld_local = RTLD_LOCAL;
int rtld_nodelete = RTLD_NODELETE;
int rtld_noload = RTLD_NOLOAD;
int rtld_deepbind = RTLD_DEEPBIND;

void* kautil_dlopen(const char * path_to_shared_lib, int flags) { return dlopen(path_to_shared_lib,flags); }
void* kautil_dlsym(void * handle, const char *symbol) { return (void *) dlsym(handle,(char *)symbol); }
int kautil_dlclose(void * handle) { return dlclose(handle); }

#endif


struct kautil_database_sqlite3_extern_fail_to_load_symbol_initialize{};
struct kautil_database_sqlite3_extern_fail_to_load_symbol_finalize{};
struct kautil_database_sqlite3_extern_fail_to_open_shared_library{};
struct auto_release{
    auto_release(const char * path,int mode){
        dl = kautil_dlopen(path,mode);
        if(nullptr==dl) throw kautil_database_sqlite3_extern_fail_to_open_shared_library{};
        initialize = (kautil::database::Sqlite3* (*)()) kautil_dlsym(dl,"extern_initialize");
        finalize = (void (*)(kautil::database::Sqlite3*)) kautil_dlsym(dl,"extern_finalize");
        if(nullptr==initialize) throw kautil_database_sqlite3_extern_fail_to_load_symbol_initialize{};
        if(nullptr==initialize) throw kautil_database_sqlite3_extern_fail_to_load_symbol_finalize{};
    }
    ~auto_release(){ kautil_dlclose(dl); }
    void * dl = 0; 
    kautil::database::Sqlite3* (*initialize)() = 0; 
    void (*finalize)(kautil::database::Sqlite3*) = 0; 
};

static auto_release * kCtrlDefaultPtr;
kautil::database::Sqlite3 * kautil_database_sqlite3_initialize(){
    auto init_once = [](){
        static auto kCtrlDefault = auto_release{KAUTIL_DATABASE_SQLITE3_SO,rtld_lazy|rtld_nodelete};
         kCtrlDefaultPtr = &kCtrlDefault;
        return true;
    }.operator()();
    return kCtrlDefaultPtr->initialize();
}
void  kautil_database_sqlite3_release(kautil::database::Sqlite3 * m){
    kCtrlDefaultPtr->finalize(m);
}


