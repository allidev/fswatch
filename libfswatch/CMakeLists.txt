#
# Copyright (c) 2014-2016 Enrico M. Crisostomo
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 3, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
#
include_directories(. src/libfswatch)

set(LIB_SOURCE_FILES
        src/libfswatch/c/cevent.cpp
        src/libfswatch/c/cevent.h
        src/libfswatch/c/cfilter.h
        src/libfswatch/c/cmonitor.h
        src/libfswatch/c/error.h
        src/libfswatch/c/libfswatch.cpp
        src/libfswatch/c/libfswatch.h
        src/libfswatch/c/libfswatch_log.cpp
        src/libfswatch/c/libfswatch_log.h
        src/libfswatch/c/libfswatch_types.h
        src/libfswatch/c++/event.cpp
        src/libfswatch/c++/event.hpp
        src/libfswatch/c++/filter.hpp
        src/libfswatch/c++/libfswatch_exception.cpp
        src/libfswatch/c++/libfswatch_exception.hpp
        src/libfswatch/c++/libfswatch_map.hpp
        src/libfswatch/c++/libfswatch_set.hpp
        src/libfswatch/c++/monitor.cpp
        src/libfswatch/c++/monitor.hpp
        src/libfswatch/c++/path_utils.cpp
        src/libfswatch/c++/path_utils.hpp
        src/libfswatch/c++/poll_monitor.cpp
        src/libfswatch/c++/poll_monitor.hpp
        src/libfswatch/c++/string/string_utils.cpp
        src/libfswatch/c++/string/string_utils.hpp
        src/libfswatch/gettext.h
        src/libfswatch/gettext_defs.h
        libfswatch_config.h)

INCLUDE(CheckIncludeFiles)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckStructHasMember)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckCXXCompilerFlag)
#INCLUDE(CheckTypeSize)

CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_CXX11)
#SET(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -std=c++11")
#SET(CMAKE_EXTRA_INCLUDE_FILES "${CMAKE_EXTRA_INCLUDE_FILES} <memory> <string>")
#CHECK_TYPE_SIZE(std::unique<std::string> CXX_UNIQUE_PTR LANGUAGE CXX)

CHECK_INCLUDE_FILES(sys/inotify.h HAVE_SYS_INOTIFY_H)

if (HAVE_SYS_INOTIFY_H)
    set(LIB_SOURCE_FILES
            ${LIB_SOURCE_FILES}
            src/libfswatch/c++/inotify_monitor.cpp
            src/libfswatch/c++/inotify_monitor.hpp)
    CHECK_SYMBOL_EXISTS(inotify_init "sys/inotify.h" HAVE_DECL_INOTIFY_INIT)
    CHECK_SYMBOL_EXISTS(inotify_add_watch "sys/inotify.h" HAVE_DECL_INOTIFY_ADD_WATCH)
    CHECK_SYMBOL_EXISTS(inotify_rm_watch "sys/inotify.h" HAVE_DECL_INOTIFY_RM_WATCH)
endif (HAVE_SYS_INOTIFY_H)

CHECK_INCLUDE_FILES(sys/event.h HAVE_SYS_EVENT_H)

if (HAVE_SYS_EVENT_H)
    set(LIB_SOURCE_FILES
            ${LIB_SOURCE_FILES}
            src/libfswatch/c++/kqueue_monitor.cpp
            src/libfswatch/c++/kqueue_monitor.hpp)
    CHECK_SYMBOL_EXISTS(kqueue "sys/event.h;sys/types.h;sys/time.h" HAVE_DECL_KQUEUE)
    CHECK_SYMBOL_EXISTS(kevent "sys/event.h;sys/types.h;sys/time.h" HAVE_DECL_KEVENT)
endif (HAVE_SYS_EVENT_H)

CHECK_INCLUDE_FILES(port.h HAVE_PORT_H)

if (HAVE_PORT_H)
    set(LIB_SOURCE_FILES
            ${LIB_SOURCE_FILES}
            src/libfswatch/c++/fen_monitor.cpp
            src/libfswatch/c++/fen_monitor.hpp)
endif (HAVE_PORT_H)

CHECK_INCLUDE_FILE_CXX(cstdlib HAVE_CSTDLIB)
CHECK_INCLUDE_FILE_CXX(atomic HAVE_CXX_ATOMIC)
CHECK_INCLUDE_FILE_CXX(mutex HAVE_CXX_MUTEX)

if (WIN32)
    CHECK_INCLUDE_FILES(sys/cygwin.h HAVE_CYGWIN)

    if (HAVE_CYGWIN)
        set(LIB_SOURCE_FILES
                ${LIB_SOURCE_FILES}
                src/libfswatch/c++/windows/win_directory_change_event.cpp
                src/libfswatch/c++/windows/win_directory_change_event.hpp
                src/libfswatch/c++/windows/win_error_message.cpp
                src/libfswatch/c++/windows/win_error_message.hpp
                src/libfswatch/c++/windows/win_handle.cpp
                src/libfswatch/c++/windows/win_handle.hpp
                src/libfswatch/c++/windows/win_paths.cpp
                src/libfswatch/c++/windows/win_paths.hpp
                src/libfswatch/c++/windows/win_strings.cpp
                src/libfswatch/c++/windows/win_strings.hpp
                src/libfswatch/c++/windows_monitor.cpp
                src/libfswatch/c++/windows_monitor.hpp)
        add_definitions(-DHAVE_WINDOWS)
    endif (HAVE_CYGWIN)
endif (WIN32)

if (UNIX AND NOT APPLE AND NOT ANDROID)
    set(EXTRA_LIBS pthread)
endif (UNIX AND NOT APPLE AND NOT ANDROID)

if (APPLE)
    CHECK_INCLUDE_FILES(CoreServices/CoreServices.h HAVE_FSEVENTS_FILE_EVENTS)

    if (HAVE_FSEVENTS_FILE_EVENTS)
        find_library(CORESERVICES_LIBRARY CoreServices)
        set(EXTRA_LIBS ${CORESERVICES_LIBRARY})

        set(LIB_SOURCE_FILES
                ${LIB_SOURCE_FILES}
                src/libfswatch/c++/fsevents_monitor.cpp
                src/libfswatch/c++/fsevents_monitor.hpp)

    endif (HAVE_FSEVENTS_FILE_EVENTS)
endif (APPLE)

CHECK_STRUCT_HAS_MEMBER("struct stat" st_mtime sys/stat.h HAVE_STRUCT_STAT_ST_MTIME)

add_library(libfswatch SHARED ${LIB_SOURCE_FILES})
SET_TARGET_PROPERTIES(libfswatch PROPERTIES PREFIX "")
configure_file(libfswatch_config.h.cmakein ${PROJECT_SOURCE_DIR}/libfswatch/libfswatch_config.h)
target_include_directories(libfswatch PUBLIC src)
target_link_libraries(libfswatch 
	${EXTRA_LIBS})
