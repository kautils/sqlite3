
set(module_name alter)
unset(srcs)
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/*.cc)
set(${module_name}_common_pref
    MODULE_PREFIX kautil sqlite3
    MODULE_NAME ${module_name}
    INCLUDES $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<INSTALL_INTERFACE:include> ${CMAKE_CURRENT_LIST_DIR} 
    SOURCES ${srcs}
    LINK_LIBS kautil::sqlite3::info::${${PROJECT_NAME}.version}::interface
    EXPORT_NAME_PREFIX ${PROJECT_NAME}Alters
    EXPORT_NAME_CMAKE_DIR ${PROJECT_NAME}
    EXPORT_VERSION ${PROJECT_VERSION}
    EXPORT_VERSION_COMPATIBILITY AnyNewerVersion
        
    DESTINATION_INCLUDE_DIR include/kautil/sqlite3
    DESTINATION_CMAKE_DIR cmake
    DESTINATION_LIB_DIR lib
)

CMakeLibraryTemplate(${module_name} EXPORT_LIB_TYPE static ${${module_name}_common_pref} )
#CMakeLibraryTemplate(${module_name} EXPORT_LIB_TYPE shared ${${module_name}_common_pref} )

set(__tmain_mc TMAIN_KAUTIL_SQLITE3_ALTER_STATIC TMAIN_KAUTIL_SQLITE3_ALTER)
set(__t ${${module_name}_static_tmain})
add_executable(${__t})
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/unit_test.cc)
target_link_libraries(${__t} PRIVATE ${${module_name}_static} ${PROJECT_NAME}.test_data)
target_compile_definitions(${__t} PRIVATE ${__tmain_mc})
