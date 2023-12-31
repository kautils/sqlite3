
set(${m}_kautil_cmake_heeder CMakeKautilHeader.v0.0.cmake)
if(DEFINED KAUTIL_THIRD_PARTY_DIR AND EXISTS "${KAUTIL_THIRD_PARTY_DIR}/${${m}_kautil_cmake_heeder}")
    include("${KAUTIL_THIRD_PARTY_DIR}/${${m}_kautil_cmake_heeder}")
else()
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/${${m}_kautil_cmake_heeder})
        file(DOWNLOAD https://raw.githubusercontent.com/kautils/CMakeKautilHeader/v0.0/CMakeKautilHeader.cmake ${CMAKE_BINARY_DIR}/${${m}_kautil_cmake_heeder})
    endif()
    include(${CMAKE_BINARY_DIR}/${${m}_kautil_cmake_heeder})
endif()
include(${CMAKE_CURRENT_LIST_DIR}/sqlite3/sqlite3.cmake)


if(${BUILD_SHARED_LIBS})
    
    unset(__main) 
    unset(__libs)
    unset(__tmain)
    unset(__tmain_libs)
    unset(__alias )
    
    set(__main kautil_sqlite3_extern_static)
    set(__alias kautil::sqlite3::extern::static)
    set(__tmain tmain_kautil_sqlite3_extern_static)
    set(__tmain_mc TMAIN_KAUTIL_SQLITE3_EXTERN_STATIC)
    
    set(__t ${__main})
    add_library(${__t} STATIC)
    unset(srcs)
    file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/*.cc)
    target_sources(${__t} PRIVATE ${srcs})
    target_link_libraries(${__t} PRIVATE ${__libs})
    target_include_directories(${__t} INTERFACE include)
    target_compile_definitions(${__t} PRIVATE KAUTIL_DATABASE_SQLITE3_SO="$<TARGET_FILE:kautil_sqlite3_${PROJECT_VERSION}_shared>")
    add_library(${__alias} ALIAS ${__t})
    
    
    set(__t ${__tmain})
    add_executable(${__t})
    target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/unit_test.cc)
    target_link_libraries(${__t} PRIVATE ${__alias})
    target_compile_definitions(${__t} PRIVATE ${__tmain_mc})
    add_dependencies(${__t} kautil_sqlite3_${PROJECT_VERSION}_shared)
    
endif()