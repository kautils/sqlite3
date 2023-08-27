


#include "preprocessors.h"
#include <sqlite3.h>


int32_t kautil::database::sqlite3::Options::sqlite_ok() { return SQLITE_OK; }
int32_t kautil::database::sqlite3::Options::sqlite_error() {    return SQLITE_ERROR;}
int32_t kautil::database::sqlite3::Options::sqlite_internal() {    return SQLITE_INTERNAL;}
int32_t kautil::database::sqlite3::Options::sqlite_perm() {    return SQLITE_PERM;}
int32_t kautil::database::sqlite3::Options::sqlite_abort() {    return SQLITE_ABORT;}
int32_t kautil::database::sqlite3::Options::sqlite_busy() {    return SQLITE_BUSY;}
int32_t kautil::database::sqlite3::Options::sqlite_locked() {    return SQLITE_LOCKED;}
int32_t kautil::database::sqlite3::Options::sqlite_nomem() {    return SQLITE_NOMEM;}
int32_t kautil::database::sqlite3::Options::sqlite_readonly() {    return SQLITE_READONLY;}
int32_t kautil::database::sqlite3::Options::sqlite_interrupt() {    return SQLITE_INTERRUPT;}
int32_t kautil::database::sqlite3::Options::sqlite_ioerr() {    return SQLITE_IOERR;}
int32_t kautil::database::sqlite3::Options::sqlite_corrupt() {    return SQLITE_CORRUPT;}
int32_t kautil::database::sqlite3::Options::sqlite_notfound() {    return SQLITE_NOTFOUND;}
int32_t kautil::database::sqlite3::Options::sqlite_full() {    return SQLITE_FULL;}
int32_t kautil::database::sqlite3::Options::sqlite_cantopen() {    return SQLITE_CANTOPEN;}
int32_t kautil::database::sqlite3::Options::sqlite_protocol() {    return SQLITE_PROTOCOL;}
int32_t kautil::database::sqlite3::Options::sqlite_empty() {    return SQLITE_EMPTY;}
int32_t kautil::database::sqlite3::Options::sqlite_schema() {    return SQLITE_SCHEMA;}
int32_t kautil::database::sqlite3::Options::sqlite_toobig() {    return SQLITE_TOOBIG;}
int32_t kautil::database::sqlite3::Options::sqlite_constraint() {    return SQLITE_CONSTRAINT;}
int32_t kautil::database::sqlite3::Options::sqlite_mismatch() {    return SQLITE_MISMATCH;}
int32_t kautil::database::sqlite3::Options::sqlite_misuse() {    return SQLITE_MISUSE;}
int32_t kautil::database::sqlite3::Options::sqlite_nolfs() {    return SQLITE_NOLFS;}
int32_t kautil::database::sqlite3::Options::sqlite_auth() {    return SQLITE_AUTH;}
int32_t kautil::database::sqlite3::Options::sqlite_format() { return SQLITE_FORMAT; }
int32_t kautil::database::sqlite3::Options::sqlite_range() { return SQLITE_RANGE; }
int32_t kautil::database::sqlite3::Options::sqlite_notadb() { return SQLITE_NOTADB; }
int32_t kautil::database::sqlite3::Options::sqlite_notice() { return SQLITE_NOTICE; }
int32_t kautil::database::sqlite3::Options::sqlite_warning() { return SQLITE_WARNING; }
int32_t kautil::database::sqlite3::Options::sqlite_row() { return SQLITE_ROW; }
int32_t kautil::database::sqlite3::Options::sqlite_done() { return SQLITE_DONE; }
int32_t kautil::database::sqlite3::Options::sqlite_error_missing_collseq() { return SQLITE_ERROR_MISSING_COLLSEQ; }
int32_t kautil::database::sqlite3::Options::sqlite_error_retry() { return SQLITE_ERROR_RETRY; }
int32_t kautil::database::sqlite3::Options::sqlite_error_snapshot() { return SQLITE_ERROR_SNAPSHOT; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_read() { return SQLITE_IOERR_READ; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_short_read() { return SQLITE_IOERR_SHORT_READ; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_write() { return SQLITE_IOERR_WRITE; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_fsync() { return SQLITE_IOERR_FSYNC; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_dir_fsync() { return SQLITE_IOERR_DIR_FSYNC; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_truncate() { return SQLITE_IOERR_TRUNCATE; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_fstat() { return SQLITE_IOERR_FSTAT; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_unlock() { return SQLITE_IOERR_UNLOCK; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_rdlock() { return SQLITE_IOERR_RDLOCK; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_delete() { return SQLITE_IOERR_DELETE; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_blocked() { return SQLITE_IOERR_BLOCKED; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_nomem() { return SQLITE_IOERR_NOMEM; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_access() { return SQLITE_IOERR_ACCESS; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_checkreservedlock() { return SQLITE_IOERR_CHECKRESERVEDLOCK; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_lock() { return SQLITE_IOERR_LOCK; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_close() { return SQLITE_IOERR_CLOSE; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_dir_close() { return SQLITE_IOERR_DIR_CLOSE; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_shmopen() { return SQLITE_IOERR_SHMOPEN; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_shmsize() { return SQLITE_IOERR_SHMSIZE; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_shmlock() { return SQLITE_IOERR_SHMLOCK; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_shmmap() { return SQLITE_IOERR_SHMMAP; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_seek() { return SQLITE_IOERR_SEEK; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_delete_noent() { return SQLITE_IOERR_DELETE_NOENT; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_mmap() { return SQLITE_IOERR_MMAP; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_gettemppath() { return SQLITE_IOERR_GETTEMPPATH; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_convpath() { return SQLITE_IOERR_CONVPATH; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_vnode() { return SQLITE_IOERR_VNODE; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_auth() { return SQLITE_IOERR_AUTH; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_begin_atomic() { return SQLITE_IOERR_BEGIN_ATOMIC; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_commit_atomic() { return SQLITE_IOERR_COMMIT_ATOMIC; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_rollback_atomic() { return SQLITE_IOERR_ROLLBACK_ATOMIC; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_data() { return SQLITE_IOERR_DATA; }
int32_t kautil::database::sqlite3::Options::sqlite_ioerr_corruptfs() { return SQLITE_IOERR_CORRUPTFS; }
int32_t kautil::database::sqlite3::Options::sqlite_locked_sharedcache() { return SQLITE_LOCKED_SHAREDCACHE; }
int32_t kautil::database::sqlite3::Options::sqlite_locked_vtab() { return SQLITE_LOCKED_VTAB; }
int32_t kautil::database::sqlite3::Options::sqlite_busy_recovery() { return SQLITE_BUSY_RECOVERY; }
int32_t kautil::database::sqlite3::Options::sqlite_busy_snapshot() { return SQLITE_BUSY_SNAPSHOT; }
int32_t kautil::database::sqlite3::Options::sqlite_busy_timeout() { return SQLITE_BUSY_TIMEOUT; }
int32_t kautil::database::sqlite3::Options::sqlite_cantopen_notempdir() { return SQLITE_CANTOPEN_NOTEMPDIR; }
int32_t kautil::database::sqlite3::Options::sqlite_cantopen_isdir() { return SQLITE_CANTOPEN_ISDIR; }
int32_t kautil::database::sqlite3::Options::sqlite_cantopen_fullpath() { return SQLITE_CANTOPEN_FULLPATH; }
int32_t kautil::database::sqlite3::Options::sqlite_cantopen_convpath() { return SQLITE_CANTOPEN_CONVPATH; }
int32_t kautil::database::sqlite3::Options::sqlite_cantopen_symlink() { return SQLITE_CANTOPEN_SYMLINK; }
int32_t kautil::database::sqlite3::Options::sqlite_corrupt_vtab() { return SQLITE_CORRUPT_VTAB; }
int32_t kautil::database::sqlite3::Options::sqlite_corrupt_sequence() { return SQLITE_CORRUPT_SEQUENCE; }
int32_t kautil::database::sqlite3::Options::sqlite_corrupt_index() { return SQLITE_CORRUPT_INDEX; }
int32_t kautil::database::sqlite3::Options::sqlite_readonly_recovery() { return SQLITE_READONLY_RECOVERY; }
int32_t kautil::database::sqlite3::Options::sqlite_readonly_cantlock() { return SQLITE_READONLY_CANTLOCK; }
int32_t kautil::database::sqlite3::Options::sqlite_readonly_rollback() { return SQLITE_READONLY_ROLLBACK; }
int32_t kautil::database::sqlite3::Options::sqlite_readonly_dbmoved() { return SQLITE_READONLY_DBMOVED; }
int32_t kautil::database::sqlite3::Options::sqlite_readonly_cantinit() { return SQLITE_READONLY_CANTINIT; }
int32_t kautil::database::sqlite3::Options::sqlite_readonly_directory() { return SQLITE_READONLY_DIRECTORY; }
int32_t kautil::database::sqlite3::Options::sqlite_abort_rollback() { return SQLITE_ABORT_ROLLBACK; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_check() { return SQLITE_CONSTRAINT_CHECK; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_commithook() { return SQLITE_CONSTRAINT_COMMITHOOK; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_foreignkey() { return SQLITE_CONSTRAINT_FOREIGNKEY; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_function() { return SQLITE_CONSTRAINT_FUNCTION; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_notnull() { return SQLITE_CONSTRAINT_NOTNULL; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_primarykey() { return SQLITE_CONSTRAINT_PRIMARYKEY; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_trigger() { return SQLITE_CONSTRAINT_TRIGGER; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_unique() { return SQLITE_CONSTRAINT_UNIQUE; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_vtab() { return SQLITE_CONSTRAINT_VTAB; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_rowid() { return SQLITE_CONSTRAINT_ROWID; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_pinned() { return SQLITE_CONSTRAINT_PINNED; }
int32_t kautil::database::sqlite3::Options::sqlite_constraint_datatype() { return SQLITE_CONSTRAINT_DATATYPE; }
int32_t kautil::database::sqlite3::Options::sqlite_notice_recover_wal() { return SQLITE_NOTICE_RECOVER_WAL; }
int32_t kautil::database::sqlite3::Options::sqlite_notice_recover_rollback() { return SQLITE_NOTICE_RECOVER_ROLLBACK; }
int32_t kautil::database::sqlite3::Options::sqlite_warning_autoindex() { return SQLITE_WARNING_AUTOINDEX; }
int32_t kautil::database::sqlite3::Options::sqlite_auth_user() { return SQLITE_AUTH_USER; }
int32_t kautil::database::sqlite3::Options::sqlite_ok_load_permanently() { return SQLITE_OK_LOAD_PERMANENTLY; }
int32_t kautil::database::sqlite3::Options::sqlite_ok_symlink() { return SQLITE_OK_SYMLINK; }
int32_t kautil::database::sqlite3::Options::sqlite_open_readonly() { return SQLITE_OPEN_READONLY; }
int32_t kautil::database::sqlite3::Options::sqlite_open_readwrite() { return SQLITE_OPEN_READWRITE; }
int32_t kautil::database::sqlite3::Options::sqlite_open_create() { return SQLITE_OPEN_CREATE; }
int32_t kautil::database::sqlite3::Options::sqlite_open_deleteonclose() { return SQLITE_OPEN_DELETEONCLOSE; }
int32_t kautil::database::sqlite3::Options::sqlite_open_exclusive() { return SQLITE_OPEN_EXCLUSIVE; }
int32_t kautil::database::sqlite3::Options::sqlite_open_autoproxy() { return SQLITE_OPEN_AUTOPROXY; }
int32_t kautil::database::sqlite3::Options::sqlite_open_uri() { return SQLITE_OPEN_URI; }
int32_t kautil::database::sqlite3::Options::sqlite_open_memory() { return SQLITE_OPEN_MEMORY; }
int32_t kautil::database::sqlite3::Options::sqlite_open_main_db() { return SQLITE_OPEN_MAIN_DB; }
int32_t kautil::database::sqlite3::Options::sqlite_open_temp_db() { return SQLITE_OPEN_TEMP_DB; }
int32_t kautil::database::sqlite3::Options::sqlite_open_transient_db() { return SQLITE_OPEN_TRANSIENT_DB; }
int32_t kautil::database::sqlite3::Options::sqlite_open_main_journal() { return SQLITE_OPEN_MAIN_JOURNAL; }
int32_t kautil::database::sqlite3::Options::sqlite_open_temp_journal() { return SQLITE_OPEN_TEMP_JOURNAL; }
int32_t kautil::database::sqlite3::Options::sqlite_open_subjournal() { return SQLITE_OPEN_SUBJOURNAL; }
int32_t kautil::database::sqlite3::Options::sqlite_open_super_journal() { return SQLITE_OPEN_SUPER_JOURNAL; }
int32_t kautil::database::sqlite3::Options::sqlite_open_nomutex() { return SQLITE_OPEN_NOMUTEX; }
int32_t kautil::database::sqlite3::Options::sqlite_open_fullmutex() { return SQLITE_OPEN_FULLMUTEX; }
int32_t kautil::database::sqlite3::Options::sqlite_open_sharedcache() { return SQLITE_OPEN_SHAREDCACHE; }
int32_t kautil::database::sqlite3::Options::sqlite_open_privatecache() { return SQLITE_OPEN_PRIVATECACHE; }
int32_t kautil::database::sqlite3::Options::sqlite_open_wal() { return SQLITE_OPEN_WAL; }
int32_t kautil::database::sqlite3::Options::sqlite_open_nofollow() { return SQLITE_OPEN_NOFOLLOW; }
int32_t kautil::database::sqlite3::Options::sqlite_open_exrescode() { return SQLITE_OPEN_EXRESCODE; }
int32_t kautil::database::sqlite3::Options::sqlite_open_master_journal() { return SQLITE_OPEN_MASTER_JOURNAL; }
int32_t kautil::database::sqlite3::Options::sqlite_deterministic() { return SQLITE_DETERMINISTIC; }
int32_t kautil::database::sqlite3::Options::sqlite_directonly() { return SQLITE_DIRECTONLY; }
int32_t kautil::database::sqlite3::Options::sqlite_subtype() { return SQLITE_SUBTYPE; }
int32_t kautil::database::sqlite3::Options::sqlite_innocuous() { return SQLITE_INNOCUOUS; }
sqlite3_destructor_type kautil::database::sqlite3::Options::sqlite_static(){ return SQLITE_STATIC; };   
sqlite3_destructor_type kautil::database::sqlite3::Options::sqlite_transient(){ return SQLITE_TRANSIENT; };   
    

void kautil::database::sqlite3::Options::release(){ delete this; }
kautil::database::sqlite3::Options::Options(){}
kautil::database::sqlite3::Options * kautil::database::sqlite3::sqlite_options(){ return new Options{}; }
