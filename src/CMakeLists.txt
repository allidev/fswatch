include_directories(.. .)

set(SOURCE_FILES
        fswatch.cpp
        fswatch.hpp
        gettext.h
        ../config.h)

set(PACKAGE "package name")
set(PACKAGE_NAME "package name")
set(PACKAGE_STRING "package string")
set(PACKAGE_BUGREPORT "package bugreport")
set(PACKAGE_URL "package url")

add_executable(fswatch ${SOURCE_FILES})
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckFunctionExists)

CHECK_INCLUDE_FILES(getopt.h HAVE_GETOPT_H)
CHECK_INCLUDE_FILES(dlfcn.h HAVE_DLFCN_H)

CHECK_FUNCTION_EXISTS(getopt_long HAVE_GETOPT_LONG)
CHECK_FUNCTION_EXISTS(atexit HAVE_ATEXIT)

configure_file(${PROJECT_SOURCE_DIR}/config.h.cmakein ${PROJECT_SOURCE_DIR}/config.h)
target_link_libraries(fswatch LINK_PUBLIC libfswatch)
