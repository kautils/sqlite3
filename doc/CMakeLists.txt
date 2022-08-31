cmake_minimum_required(VERSION 3.20)
project(ThirdParty
        LANGUAGES C CXX
        VERSION 0.0.0.0
        #[[HOMEPAGE_URL https://***someurl***]]
        #[[DESCRIPTION ""]]
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
set(CMAKE_C_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_FLAGS -O2)


include(cmake/functions.cmake)


KautilGenerateDefaultProjectTree()
set(lib_target sqlite3)

if(USING_MINGW)
        add_library(${lib_target} STATIC sqlite3.c)
        set_target_properties(${lib_target} PROPERTIES SUFFIX .lib)
else()
        add_library(${lib_target} SHARED sqlite3.c)
        set_target_properties(${lib_target} PROPERTIES SUFFIX .so)
endif()

#add_library(${lib_target} STATIC sqlite3.c)
#target_include_directories(${lib_target} INTERFACE $<INSTALL_INTERFACE:include>)
target_include_directories(${lib_target} 
        INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )
target_include_directories(${lib_target} INTERFACE 
        $<INSTALL_INTERFACE:include> # for install command
        )
#target_include_directories(${lib_target} INTERFACE $<INSTALL_INTERFACE:include>)


##if(USING_MINGW)
##        add_library(${lib_target} STATIC sqlite3.c)
##        set_target_properties(${lib_target} PROPERTIES SUFFIX .lib)
##endif()
#if(USING_MINGW)
#        add_library(${lib_target} SHARED sqlite3.c)
#        set_target_properties(${lib_target} PROPERTIES SUFFIX .dll)
#endif()


######################### START Fill ########################
set(__lib_namespace ThirdParty)
set(__lib_component_name sqlite3)
set(__lib_target    ${lib_target})
set(__lib_dst       lib64)
set(__include_srcs  ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.h )
set(__include_dst   include)
######################### END Fill ########################

KautilMakeProjectDefaultExportable(
        ${__lib_namespace}    
        ${__lib_component_name}
        ${__lib_target}
        ${__lib_dst}
        ${__include_dst}
        ${__include_srcs}
        )




#set(libs_find_pkg __some__::__some__)
#set(path_to_externals)
#set(path_to_imports ${path_to_externals}/__SOME__/lib64/cmake)
#find_package(__SOME__ x.x.x REQUIRED PATHS ${path_to_imports} NO_DEFAULT_PATH)

## EXECUTABLE 
#    set(target_name __SOME_TARGET_NAME)
#    set(srcs __SOME_SRC)
#    add_executable(${target_name} ${srcs})
#    target_include_directories(${target_name} PRIVATE include)

## LIBRARY FOR EXPORT 
#    set(lib_target __SOME_LIB_NAME)
#    add_library(${lib_target} SHARED __SOME_SRC__)
#    target_include_directories(${lib_target} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/__SOME_INCLUDE_DIR>)
#    target_include_directories(${lib_target} INTERFACE $<INSTALL_INTERFACE:__SOME_INCLUDE_DIR>)
#    target_precompile_headers(${lib_target}  PRIVATE __SOME_HEADER)
#    target_link_libraries(${lib_target} PRIVATE __SOME_LIBRARIES)
