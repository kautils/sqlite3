unset(__main) 
unset(__libs)
unset(__tmain)
unset(__tmain_libs)
unset(__alias )

set(__main kautil_sqlite_stmt_static)
set(__alias kautil::sqlite3::stmt::static)
set(__tmain tmain_kautil_sqlite_stmt_static)
set(__tmain_mc TMAIN_KAUTIL_SQLITE_STMT_STATIC)
set(__libs ${KAUTIL_LIBSQLITE3_LIBNAME})
set(__include ${KAUTIL_LIBSQLITE3_INCLUDE_DIR})

set(__t ${__main})
add_library(${__t} STATIC)
unset(srcs)
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/*.cc)
target_sources(${__t} PRIVATE ${srcs})
target_include_directories(${__t} PRIVATE ${__include})
add_library(${__alias} ALIAS ${__t})

set(__t ${__tmain})
add_executable(${__t})
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/unit_test.cc)
target_link_directories(${__t} PRIVATE ${KAUTIL_LIBSQLITE3_LIBDIR})
target_link_libraries(${__t} PUBLIC ${__alias} kautil::sqlite3::test_data::static kautil::c11_string_allocator::0.0.1::static )
target_compile_definitions(${__t} PRIVATE ${__tmain_mc})



