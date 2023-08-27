

set(system_lib_sqlite_shared libsqlite3-0.dll)


unset(srcs)
set(__t kautil_sqlite_serialize )
add_library(${__t} STATIC)
file(GLOB_RECURSE srcs ${CMAKE_CURRENT_SOURCE_DIR}/include/kautil/sqlite3/serialize/*.cc)
target_sources(${__t} PRIVATE ${srcs})
target_link_libraries(${__t} PRIVATE ${system_lib_sqlite_shared})
target_include_directories(${__t} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(kautil::sqlite::serialize::static ALIAS ${__t})
unset(srcs)

set(__t tmain_kautil_sqlite_serialize)
add_executable(${__t})
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/kautil/sqlite3/serialize/unit_test.cc)
target_link_libraries(${__t} PRIVATE kautil::sqlite3::shared kautil::sqlite::serialize::static)
target_compile_definitions(${__t} PRIVATE TMAIN_KAUTIL_SQLITE_SERIALIZE)


set(__t kautil_database_wsqlite3)
file(GLOB_RECURSE srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/kautil/sqlite3/src/*cc)
add_library(${__t} STATIC)
target_sources(${__t} PRIVATE ${srcs})
target_link_directories(${__t} PRIVATE ${__mingw_root}/bin )
target_link_directories(${__t} INTERFACE ${__mingw_root}/bin )
target_link_libraries(${__t} PRIVATE ${system_lib_sqlite_shared} kautil::sqlite::serialize::static)
target_link_libraries(${__t} INTERFACE ${system_lib_sqlite_shared})
target_include_directories(${__t} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${__mingw_root}/include)
target_include_directories(${__t} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include ${__mingw_root}/include)
add_library(kautil::sqlite3::shared ALIAS ${__t})

set(__t kautil_database_wsqlite3_blob_unit_test_general_usage)
add_executable(${__t} ${CMAKE_CURRENT_SOURCE_DIR}/src/kautil/sqlite3/src/blob.cc)
target_link_libraries(${__t} kautil::sqlite3::shared)
target_compile_definitions(${__t} PRIVATE KAUTIL_SQLITE_BLOB_UNIT_TEST)


set(__t example_kautil_sqlite3_1cc)
add_executable(${__t})
target_sources(${__t} PRIVATE src/kautil/sqlite3/example/1.cc)
target_link_directories(${__t} PRIVATE ${__mingw_root}/bin)
target_link_libraries(${__t} PRIVATE kautil_sqlite3 )


# todo :  replace with Blob and erase
set(__t example_kautil_sqlite3_blob_rw)
add_executable(${__t})
target_sources(${__t} PRIVATE src/kautil/sqlite3/example/sqlite3_blob_rw.cc)
target_link_directories(${__t} PRIVATE ${__mingw_root}/bin)
target_link_libraries(${__t} PRIVATE kautil_sqlite3 )

set(__t example_kautil_sqlite3_alter_table)
add_executable(${__t})
target_sources(${__t} PRIVATE src/kautil/sqlite3/example/sqlite3_alter_table.cc)
target_link_directories(${__t} PRIVATE ${__mingw_root}/bin)
target_link_libraries(${__t} PRIVATE kautil_sqlite3 )


