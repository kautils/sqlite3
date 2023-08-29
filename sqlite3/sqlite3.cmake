
include(${CMAKE_CURRENT_LIST_DIR}/test_data/test_data.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/stmt/stmt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/blob/blob.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/serialize/serialize.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/preprocessors/preprocessors.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/alter/alter.cmake)


unset(__main) 
unset(__libs)
unset(__tmain)
unset(__tmain_libs)
unset(__alias )
set(__main kautil_sqlite3_shared)
set(__alias kautil::sqlite3::shared)
set(__tmain tmain_kautil_sqlite3_shared)
set(__tmain_mc TMAIN_KAUTIL_SQLITE3_SHARED)
set(__libs ${KAUTIL_LIBSQLITE3_LIBNAME} 
        kautil::sqlite3::stmt::static
        kautil::sqlite3::blob::static
        kautil::sqlite3::serialize::static
        kautil::sqlite3::preprocesssors::static
        kautil::sqlite3::alter::static
        kautil::c11_string_allocator::0.0.1::static
        )

set(__t ${__main})
add_library(${__t} SHARED)
unset(srcs)
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/*.cc)


target_sources(${__t} PRIVATE ${srcs})
target_link_directories(${__t} PRIVATE "${KAUTIL_LIBSQLITE3_LIBDIR}")
target_link_directories(${__t} PRIVATE "${KAUTIL_LIBSQLITE3_INCLUDE_DIR}")
target_link_libraries(${__t} PRIVATE ${__libs})
add_library(${__alias} ALIAS ${__t})


set(__t ${__tmain})
add_executable(${__t})
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/unit_test.cc)
target_link_libraries(${__t} PRIVATE ${__alias} kautil::sqlite3::test_data::static)
target_compile_definitions(${__t} PRIVATE ${__tmain_mc})



