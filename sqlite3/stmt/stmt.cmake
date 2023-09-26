
if(NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
    file(DOWNLOAD https://raw.githubusercontent.com/kautils/CMakeKautilHeader/v0.0.1/CMakeKautilHeader.cmake ${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
endif()
include(${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
git_clone(https://raw.githubusercontent.com/kautils/CMakeLibrarytemplate/v0.0.1/CMakeLibrarytemplate.cmake)


set(module_name stmt)
unset(srcs)
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/*.cc)
set(${module_name}_common_pref
    MODULE_PREFIX kautil sqlite3 
    MODULE_NAME ${module_name}
    INCLUDES $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<INSTALL_INTERFACE:include> ${CMAKE_CURRENT_LIST_DIR} 
    SOURCES ${srcs}
    LINK_LIBS kautil::sqlite3::info::${${PROJECT_NAME}.version}::interface
    EXPORT_NAME_PREFIX ${PROJECT_NAME}Stmt
    EXPORT_VERSION ${PROJECT_VERSION}
    EXPORT_VERSION_COMPATIBILITY AnyNewerVersion
        
    DESTINATION_INCLUDE_DIR include/kautil/sqlite3
    DESTINATION_CMAKE_DIR cmake
    DESTINATION_LIB_DIR lib
)

CMakeLibraryTemplate(${module_name} EXPORT_LIB_TYPE static ${${module_name}_common_pref} )
#CMakeLibraryTemplate(${module_name} EXPORT_LIB_TYPE shared ${${module_name}_common_pref} )

set(__tmain_mc TMAIN_KAUTIL_SQLITE_STMT_STATIC)
set(__t ${${module_name}_static_tmain})
add_executable(${__t})
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/unit_test.cc)
target_link_directories(${__t} PRIVATE ${KAUTIL_LIBSQLITE3_LIBDIR})
target_link_libraries(${__t} PUBLIC ${${module_name}_static} ${PROJECT_NAME}.test_data kautil::c11_string_allocator::0.0.1::static )
target_compile_definitions(${__t} PRIVATE ${__tmain_mc})



