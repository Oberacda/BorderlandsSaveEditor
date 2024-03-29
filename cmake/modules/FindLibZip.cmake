# CMake module to search for libzip
#
# Once done this will define
#
#  LIBZIP_FOUND - system has the zip library
#  LIBZIP_INCLUDE_DIRS - the zip include directories
#  LIBZIP_LIBRARY - Link this to use the zip library
#
# Copyright (c) 2017, Paul Blottiere, <paul.blottiere@oslandia.com>
# Copyright (c) 2017, Larry Shaffer, <lshaffer (at) boundlessgeo (dot) com>
#   Add support for finding zipconf.h in separate location, e.g. on macOS
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH(LIBZIP_INCLUDE_DIR
        zip.h
        PATHS
        /usr/local/include
        /usr/include
        "$ENV{LIB_DIR}/include"
        "$ENV{INCLUDE}"
        )

FIND_PATH(LIBZIP_CONF_INCLUDE_DIR
        zipconf.h
        PATHS
        /usr/local/lib/libzip/include
        /usr/lib/libzip/include
        "$ENV{LIB_DIR}/lib/libzip/include"
        "$ENV{LIB}/lib/libzip/include"
        /usr/local/include
        /usr/include
        "$ENV{LIB_DIR}/include"
        "$ENV{INCLUDE}"
        )

FIND_LIBRARY(LIBZIP_LIBRARY
        NAMES zip
        PATHS
        /usr/local/lib
        /usr/lib
        "$ENV{LIB_DIR}/lib"
        "$ENV{LIB}"
        )

IF (LIBZIP_LIBRARY AND LIBZIP_INCLUDE_DIR AND LIBZIP_CONF_INCLUDE_DIR)
    SET(LIBZIP_FOUND TRUE)
    SET(LIBZIP_INCLUDE_DIRS ${LIBZIP_INCLUDE_DIR} ${LIBZIP_CONF_INCLUDE_DIR})
    get_filename_component(LIBZIP_LIBRARY_DIR ${LIBZIP_LIBRARY} DIRECTORY)
ENDIF (LIBZIP_LIBRARY AND LIBZIP_INCLUDE_DIR AND LIBZIP_CONF_INCLUDE_DIR)

IF (LIBZIP_FOUND)
    MESSAGE(STATUS "Found libzip: ${LIBZIP_LIBRARY}")
ELSE (LIPZIP_FOUND)
    MESSAGE(FATAL_ERROR "Could not find libzip")
ENDIF (LIBZIP_FOUND)