set(${PROJECT_NAME}_m_evacu ${m})
set(m ${PROJECT_NAME}_vars)
list(APPEND ${m}_unsetter )


set(${m}_sqlite3_version 3.42.0)
set(KAUTIL_LIBSQLITE3_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mingw-w64-x86_64-sqlite3_${${m}_sqlite3_version}/lib CACHE STRING "path to libdir which includes sqlite" FORCE)
set(KAUTIL_LIBSQLITE3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mingw-w64-x86_64-sqlite3_${${m}_sqlite3_version}/include CACHE STRING "name of sqlite3 lib" FORCE)
set(KAUTIL_LIBSQLITE3_LIBNAME sqlite3_${${m}_sqlite3_version} CACHE STRING "name of sqlite3 lib" FORCE)

set(module_name info)
unset(srcs)
set(${module_name}_common_pref
    MODULE_PREFIX kautil sqlite3
    MODULE_NAME ${module_name}
    INCLUDES $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${KAUTIL_LIBSQLITE3_INCLUDE_DIR}> 
    EXPORT_NAME_PREFIX ${PROJECT_NAME}Info
    EXPORT_NAME_CMAKE_DIR ${PROJECT_NAME}
    EXPORT_VERSION ${PROJECT_VERSION}
    EXPORT_VERSION_COMPATIBILITY AnyNewerVersion
    DESTINATION_INCLUDE_DIR include/kautil/sqlite3
    DESTINATION_CMAKE_DIR cmake
    DESTINATION_LIB_DIR lib
)

set(${m}_sqlite_lib_a ${KAUTIL_LIBSQLITE3_LIBDIR}/libsqlite3_${${m}_sqlite3_version}${CMAKE_STATIC_LIBRARY_SUFFIX})
if(NOT EXISTS ${${m}_sqlite_lib_a})
    file(COPY_FILE ${KAUTIL_LIBSQLITE3_LIBDIR}/libsqlite3${CMAKE_STATIC_LIBRARY_SUFFIX} ${${m}_sqlite_lib_a})
endif()

install(FILES "${KAUTIL_LIBSQLITE3_LIBDIR}/libsqlite3_${${m}_sqlite3_version}${CMAKE_STATIC_LIBRARY_SUFFIX}" DESTINATION lib)
file(GLOB ${m}_sqlite3_includes ${KAUTIL_LIBSQLITE3_INCLUDE_DIR}/*.h)
install(FILES ${${m}_sqlite3_includes} DESTINATION include/kautil/sqlite3/org)

CMakeLibraryTemplate(${module_name} EXPORT_LIB_TYPE interface ${${module_name}_common_pref} )
target_link_libraries(${${module_name}_interface} INTERFACE $<BUILD_INTERFACE:${KAUTIL_LIBSQLITE3_LIBDIR}/lib${KAUTIL_LIBSQLITE3_LIBNAME}${CMAKE_STATIC_LIBRARY_SUFFIX}>)
target_link_libraries(${${module_name}_interface} INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/lib/lib${KAUTIL_LIBSQLITE3_LIBNAME}${CMAKE_STATIC_LIBRARY_SUFFIX}>)


foreach(__v ${${m}_unsetter})
    unset(${__v})
endforeach()
unset(${m}_unsetter)
set(m ${${PROJECT_NAME}_m_evacu})



