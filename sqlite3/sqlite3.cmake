set(m ${PROJECT_NAME}_m_evacu)
list(APPEND ${m}_unsetter )

if(NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
    file(DOWNLOAD https://raw.githubusercontent.com/kautils/CMakeKautilHeader/v0.0.1/CMakeKautilHeader.cmake ${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
endif()
include(${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
git_clone(https://raw.githubusercontent.com/kautils/CMakeLibrarytemplate/v0.0.1/CMakeLibrarytemplate.cmake)
git_clone(https://raw.githubusercontent.com/kautils/CMakeFetchKautilModule/v0.0.1/CMakeFetchKautilModule.cmake)
git_clone(https://raw.githubusercontent.com/kautils/CMakeFindKautilModule/v0.0.1/CMakeFindKautilModule.cmake)

if(${BUILD_SHARED_LIBS})
    set(${m}_install_dependencies NO_INSTALL)
endif()

CMakeFetchKautilModule(c11_string_allocator GIT https://github.com/kautils/c11_string_allocator.git REMOTE origin TAG v0.0.1)
CMakeFindKautilModule(c11_string_allocator NAME KautilC11StringAllocator.0.0.1.static ${${m}_install_dependencies})


include(${CMAKE_CURRENT_LIST_DIR}/info/info.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/test_data/test_data.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/blob/blob.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/serialize/serialize.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/preprocessors/preprocessors.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/alter/alter.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/stmt/stmt.cmake)

set(${m}_unsetter module_name srcs libs)
set(module_name sqlite3)
get_filename_component(__include_dir "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/*.cc)
set(libs 
    kautil::sqlite3::info::${${PROJECT_NAME}.version}::interface
    kautil::sqlite3::stmt::${${PROJECT_NAME}.version}::static
    kautil::sqlite3::blob::${${PROJECT_NAME}.version}::static
    kautil::sqlite3::serialize::${${PROJECT_NAME}.version}::static
    kautil::sqlite3::preprocessors::${${PROJECT_NAME}.version}::static
    kautil::sqlite3::alter::${${PROJECT_NAME}.version}::static
    kautil::c11_string_allocator::0.0.1::static)

list(APPEND ${m}_unsetter ${m}_findpkgs ${m}_prfx ${m}_sqlite3_headers)
string(APPEND ${m}_findpkgs
        
    "${c11_string_allocator.CONFIGURE_STRING}"
        
    "set(@PROJECT_NAME@Info.interface_DIR ${CMAKE_CURRENT_LIST_DIR})\n"
    "find_package(@PROJECT_NAME@Info.interface REQUIRED)\n\n"
    
    "set(@PROJECT_NAME@Alters.static_DIR ${CMAKE_CURRENT_LIST_DIR})\n"
    "find_package(@PROJECT_NAME@Alters.static REQUIRED)\n\n"
    
    "set(@PROJECT_NAME@Stmt.static_DIR ${CMAKE_CURRENT_LIST_DIR})\n"
    "find_package(@PROJECT_NAME@Stmt.static REQUIRED)\n\n"
    
    "set(@PROJECT_NAME@Blob.static_DIR ${CMAKE_CURRENT_LIST_DIR})\n"
    "find_package(@PROJECT_NAME@Blob.static REQUIRED)\n\n"
    
    "set(@PROJECT_NAME@Serialize.static_DIR ${CMAKE_CURRENT_LIST_DIR})\n"
    "find_package(@PROJECT_NAME@Serialize.static REQUIRED)\n\n"
    
    "set(@PROJECT_NAME@Preprocessors.static_DIR ${CMAKE_CURRENT_LIST_DIR})\n"
    "find_package(@PROJECT_NAME@Preprocessors.static REQUIRED)\n\n"
)


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

file(GLOB ${m}_sqlite3_headers ${CMAKE_CURRENT_LIST_DIR}/*.h)
install(FILES ${${m}_sqlite3_headers} DESTINATION include/kautil/sqlite3)

if(${BUILD_SHARED_LIBS})
    set(${m}_libtype shared )
else()
    set(${m}_libtype static )
    list(APPEND ${module_name}_common_pref EXPORT_CONFIG_IN_ADDITIONAL_CONTENT_BEFORE ${${m}_findpkgs})
endif()

CMakeLibraryTemplate(${module_name} EXPORT_LIB_TYPE ${__lib} ${${module_name}_common_pref})
target_link_directories(${${module_name}_${__lib}} PRIVATE ${KAUTIL_LIBSQLITE3_LIBDIR})


foreach(__v ${${m}_unsetter})
    unset(${__v})
endforeach()
unset(${m}_unsetter)
set(m ${${PROJECT_NAME}_m_evacu})




 
