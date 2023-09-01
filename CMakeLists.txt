cmake_minimum_required(VERSION 3.20)
project(KautilSqlite3
        LANGUAGES C CXX
        VERSION 2.0.1.0
        )
set(CMAKE_CXX_STANDARD 17)


set(KAUTIL_LIBSQLITE3_LIBDIR ${CMAKE_CURRENT_LIST_DIR}/third_party/mingw-w64-x86_64-sqlite3_3.42.0/bin)
set(KAUTIL_LIBSQLITE3_LIBNAME sqlite3)


if(NOT DEFINED KAUTIL_THIRD_PARTY_DIR)
    set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_BINARY_DIR})
    file(MAKE_DIRECTORY "${KAUTIL_THIRD_PARTY_DIR}")
endif()
if(NOT DEFINED KAUTIL_THREAD_MAX)
    set(KAUTIL_THREAD_MAX 1)
endif()


if(NOT EXISTS ${KAUTIL_THIRD_PARTY_DIR}/kautil_cmake/CMakeFetchKautilModule.cmake)
    file(DOWNLOAD https://raw.githubusercontent.com/kautils/CMakeFetchKautilModule/v0.0.1/CMakeFetchKautilModule.cmake "${KAUTIL_THIRD_PARTY_DIR}/kautil_cmake/CMakeFetchKautilModule.cmake")
endif()
include(${KAUTIL_THIRD_PARTY_DIR}/kautil_cmake/CMakeFetchKautilModule.cmake)


CMakeFetchKautilModule(c11_string_allocator
        GIT https://github.com/kautils/c11_string_allocator.git 
        REMOTE origin 
        TAG v0.0.1)


find_package(KautilC11StringAllocator.0.0.1.static REQUIRED)
include(${CMAKE_CURRENT_LIST_DIR}/extern.cmake)




