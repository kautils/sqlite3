
cmake_minimum_required(VERSION 3.20)
project(Kautil
        LANGUAGES C CXX
        VERSION 0.0.0.0
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)



if(DEFINED CMAKE_DST)
    set(__cmake_dst ${CMAKE_DST})
else()
    set(__cmake_dst ${CMAKE_CURRENT_SOURCE_DIR})
endif()

include(FetchContent)
if(NOT DEFINED kautil_cmake_POPULATED)
    FetchContent_Declare(kautil_cmake
            GIT_REPOSITORY https://source.developers.google.com/p/repos-349514/r/kautil_cmake
            GIT_TAG        master
            GIT_SHALLOW True
            UPDATE_COMMAND ""
            SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/third_party/projects/kautil_cmake
            )
    FetchContent_MakeAvailable(kautil_cmake)
    list(APPEND fetch_content kautil_cmake)
endif()

include(${kautil_cmake_SOURCE_DIR}/functions/functions.cmake)
set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/projects)
KautilInclude(PROJECT_HEADER)



if(NOT DEFINED sqlite_sqlite_POPULATED)
    FetchContent_Declare(sqlite_sqlite
        GIT_REPOSITORY https://source.developers.google.com/p/c-and-cpp/r/sqlite
        GIT_TAG master
        GIT_SHALLOW True
        UPDATE_COMMAND ""
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/projects/sqlite
        )
    list(APPEND fetch_content sqlite_sqlite)
endif()
FetchContent_MakeAvailable(${fetch_content})


list(APPEND find_packages sqlite_sqlite)
KautilSetPackageDir("${find_packages}" CMAKE_DIR ${__cmake_dst})
foreach(pkg IN LISTS find_packages)
    KautilForceGenerateTargetsCMake(
            PACKAGE ${pkg}
            CMAKE_DST ${__cmake_dst}
            CONFIGURE_COMMAND
            cmake
            -S ${kautil_general_SOURCE_DIR}
            -B ${kautil_general_BINARY_DIR}
            -DCMAKE_DST=${__cmake_dst}
    )
    find_package(${pkg} REQUIRED)
endforeach()
KautilUnSetPackageDir("${find_packages}" )

file(GLOB __sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
KautilExportableLib(
    COMPONENT sqlite3
    NAMESPACE kautil
    TARGET sqlite3
    OUTPUT_NAME sqlite3
    TARGET_SOURCES ${__sources}
    TARGET_LINK_MODULES sqlite::sqlite
    INSTALL_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
    INCLUDE_DST include/kautil/sqlite3
    CMAKE_DST ${__cmake_dst}
)



#add_custom_target(
#        kautil_build ALL
#        DEPENDS sqlite3_shared sqlite3
#)





