
#ifndef KAUTIL_SQLITE3_PREPROCESSORS_PREPROCESSORS_H
#define KAUTIL_SQLITE3_PREPROCESSORS_PREPROCESSORS_H

#include <stdint.h>

namespace kautil{
namespace database{
namespace sqlite3{

struct Options{
    
    
    virtual ~Options()=default;
    virtual int32_t sqlite_ok();
    virtual int32_t sqlite_error();
    virtual int32_t sqlite_internal();
    virtual int32_t sqlite_perm();
    virtual int32_t sqlite_abort();
    virtual int32_t sqlite_busy();
    virtual int32_t sqlite_locked();
    virtual int32_t sqlite_nomem();
    virtual int32_t sqlite_readonly();
    virtual int32_t sqlite_interrupt();
    virtual int32_t sqlite_ioerr();
    virtual int32_t sqlite_corrupt();
    virtual int32_t sqlite_notfound();
    virtual int32_t sqlite_full();
    virtual int32_t sqlite_cantopen();
    virtual int32_t sqlite_protocol();
    virtual int32_t sqlite_empty();
    virtual int32_t sqlite_schema();
    virtual int32_t sqlite_toobig();
    virtual int32_t sqlite_constraint();
    virtual int32_t sqlite_mismatch();
    virtual int32_t sqlite_misuse();
    virtual int32_t sqlite_nolfs();
    virtual int32_t sqlite_auth();
    virtual int32_t sqlite_format();
    virtual int32_t sqlite_range();
    virtual int32_t sqlite_notadb();
    virtual int32_t sqlite_notice();
    virtual int32_t sqlite_warning();
    virtual int32_t sqlite_row();
    virtual int32_t sqlite_done();
    
    
    virtual int32_t sqlite_error_missing_collseq ();
    virtual int32_t sqlite_error_retry ();
    virtual int32_t sqlite_error_snapshot();
    virtual int32_t sqlite_ioerr_read();
    virtual int32_t sqlite_ioerr_short_read();
    virtual int32_t sqlite_ioerr_write ();
    virtual int32_t sqlite_ioerr_fsync ();
    virtual int32_t sqlite_ioerr_dir_fsync();
    virtual int32_t sqlite_ioerr_truncate();
    virtual int32_t sqlite_ioerr_fstat ();
    virtual int32_t sqlite_ioerr_unlock();
    virtual int32_t sqlite_ioerr_rdlock();
    virtual int32_t sqlite_ioerr_delete();
    virtual int32_t sqlite_ioerr_blocked();
    virtual int32_t sqlite_ioerr_nomem ();
    virtual int32_t sqlite_ioerr_access();
    virtual int32_t sqlite_ioerr_checkreservedlock();
    virtual int32_t sqlite_ioerr_lock();
    virtual int32_t sqlite_ioerr_close ();
    virtual int32_t sqlite_ioerr_dir_close();
    virtual int32_t sqlite_ioerr_shmopen();
    virtual int32_t sqlite_ioerr_shmsize();
    virtual int32_t sqlite_ioerr_shmlock();
    virtual int32_t sqlite_ioerr_shmmap();
    virtual int32_t sqlite_ioerr_seek();
    virtual int32_t sqlite_ioerr_delete_noent();
    virtual int32_t sqlite_ioerr_mmap();
    virtual int32_t sqlite_ioerr_gettemppath();
    virtual int32_t sqlite_ioerr_convpath();
    virtual int32_t sqlite_ioerr_vnode ();
    virtual int32_t sqlite_ioerr_auth();
    virtual int32_t sqlite_ioerr_begin_atomic();
    virtual int32_t sqlite_ioerr_commit_atomic();
    virtual int32_t sqlite_ioerr_rollback_atomic ();
    virtual int32_t sqlite_ioerr_data();
    virtual int32_t sqlite_ioerr_corruptfs();
    virtual int32_t sqlite_locked_sharedcache();
    virtual int32_t sqlite_locked_vtab ();
    virtual int32_t sqlite_busy_recovery();
    virtual int32_t sqlite_busy_snapshot();
    virtual int32_t sqlite_busy_timeout();
    virtual int32_t sqlite_cantopen_notempdir();
    virtual int32_t sqlite_cantopen_isdir();
    virtual int32_t sqlite_cantopen_fullpath();
    virtual int32_t sqlite_cantopen_convpath();
    virtual int32_t sqlite_cantopen_symlink();
    virtual int32_t sqlite_corrupt_vtab();
    virtual int32_t sqlite_corrupt_sequence();
    virtual int32_t sqlite_corrupt_index();
    virtual int32_t sqlite_readonly_recovery();
    virtual int32_t sqlite_readonly_cantlock();
    virtual int32_t sqlite_readonly_rollback();
    virtual int32_t sqlite_readonly_dbmoved();
    virtual int32_t sqlite_readonly_cantinit();
    virtual int32_t sqlite_readonly_directory();
    virtual int32_t sqlite_abort_rollback();
    virtual int32_t sqlite_constraint_check();
    virtual int32_t sqlite_constraint_commithook ();
    virtual int32_t sqlite_constraint_foreignkey ();
    virtual int32_t sqlite_constraint_function();
    virtual int32_t sqlite_constraint_notnull();
    virtual int32_t sqlite_constraint_primarykey ();
    virtual int32_t sqlite_constraint_trigger();
    virtual int32_t sqlite_constraint_unique();
    virtual int32_t sqlite_constraint_vtab();
    virtual int32_t sqlite_constraint_rowid();
    virtual int32_t sqlite_constraint_pinned();
    virtual int32_t sqlite_constraint_datatype();
    virtual int32_t sqlite_notice_recover_wal();
    virtual int32_t sqlite_notice_recover_rollback();
    virtual int32_t sqlite_warning_autoindex();
    virtual int32_t sqlite_auth_user();
    virtual int32_t sqlite_ok_load_permanently();
    virtual int32_t sqlite_ok_symlink();
    
    virtual int32_t sqlite_open_readonly();
    virtual int32_t sqlite_open_readwrite();
    virtual int32_t sqlite_open_create();
    virtual int32_t sqlite_open_deleteonclose();
    virtual int32_t sqlite_open_exclusive();
    virtual int32_t sqlite_open_autoproxy();
    virtual int32_t sqlite_open_uri();
    virtual int32_t sqlite_open_memory();
    virtual int32_t sqlite_open_main_db();
    virtual int32_t sqlite_open_temp_db();
    virtual int32_t sqlite_open_transient_db();
    virtual int32_t sqlite_open_main_journal();
    virtual int32_t sqlite_open_temp_journal();
    virtual int32_t sqlite_open_subjournal();
    virtual int32_t sqlite_open_super_journal();
    virtual int32_t sqlite_open_nomutex();
    virtual int32_t sqlite_open_fullmutex();
    virtual int32_t sqlite_open_sharedcache();
    virtual int32_t sqlite_open_privatecache();
    virtual int32_t sqlite_open_wal();
    virtual int32_t sqlite_open_nofollow();
    virtual int32_t sqlite_open_exrescode();
    virtual int32_t sqlite_open_master_journal();
    
    virtual int32_t sqlite_deterministic();
    virtual int32_t sqlite_directonly();
    virtual int32_t sqlite_subtype();
    virtual int32_t sqlite_innocuous();
        
    typedef void (*sqlite3_destructor_type)(void*);
    virtual sqlite3_destructor_type sqlite_static();   
    virtual sqlite3_destructor_type sqlite_transient();   
    
    virtual void release();
    friend Options * sqlite_options();
    
private:
    Options();
};


Options * sqlite_options();


} //namespace sqlite3{
} //namespace database{
} //namespace kautil{




#endif