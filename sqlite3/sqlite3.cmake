
include(${CMAKE_CURRENT_LIST_DIR}/test_data/test_data.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/blob/blob.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/serialize/serialize.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/preprocessors/preprocessors.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/alter/alter.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/stmt/stmt.cmake)


if(NOT DEFINED ${KAUTIL_THIRD_PARTY_DIR})
    set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_BINARY_DIR})
    file(MAKE_DIRECTORY "${KAUTIL_THIRD_PARTY_DIR}")
endif()

macro(git_clone url)
    get_filename_component(file_name ${url} NAME)
    if(NOT EXISTS ${KAUTIL_THIRD_PARTY_DIR}/kautil_cmake/${file_name})
        file(DOWNLOAD ${url} "${KAUTIL_THIRD_PARTY_DIR}/kautil_cmake/${file_name}")
    endif()
    include("${KAUTIL_THIRD_PARTY_DIR}/kautil_cmake/${file_name}")
    unset(file_name)
endmacro()

git_clone(https://raw.githubusercontent.com/kautils/CMakeLibrarytemplate/v0.0.1/CMakeLibrarytemplate.cmake)

set(unsetter module_name srcs libs)
set(module_name sqlite3)
get_filename_component(__include_dir "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/*.cc)
set(libs ${KAUTIL_LIBSQLITE3_LIBNAME}
    kautil::sqlite3::stmt::static
    kautil::sqlite3::blob::static
    kautil::sqlite3::serialize::static
    kautil::sqlite3::preprocesssors::static
    kautil::sqlite3::alter::static
    kautil::c11_string_allocator::0.0.1::static)

set(${module_name}_common_pref
    MODULE_PREFIX kautil
    MODULE_NAME ${module_name}
    INCLUDES $<BUILD_INTERFACE:${__include_dir}> $<INSTALL_INTERFACE:include> 
    SOURCES ${srcs}
    LINK_LIBS ${libs}
    EXPORT_NAME_PREFIX ${PROJECT_NAME}
    EXPORT_VERSION ${PROJECT_VERSION}
    EXPORT_VERSION_COMPATIBILITY AnyNewerVersion
        
    DESTINATION_INCLUDE_DIR include
    DESTINATION_CMAKE_DIR cmake
    DESTINATION_LIB_DIR lib
)


CMakeLibraryTemplate(${module_name} EXPORT_LIB_TYPE shared ${${module_name}_common_pref} )
target_link_directories(${${module_name}_shared} PRIVATE ${KAUTIL_LIBSQLITE3_LIBDIR})


set(__t ${${module_name}_shared_tmain})
add_executable(${__t})
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/unit_test.cc)
target_link_libraries(${__t} PRIVATE ${${module_name}_shared} kautil::sqlite3::test_data::static)
target_compile_definitions(${__t} PRIVATE ${${module_name}_shared_tmain_ppcs})

foreach(__var ${unsetter})
    unset(${__var})
endforeach()




 