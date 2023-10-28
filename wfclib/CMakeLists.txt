cmake_minimum_required(VERSION 3.13)

project(wfclib VERSION 1.0.1 DESCRIPTION "wave function collapse library")

set(WFCLIB_SOURCES
    src/wfclib.c
    src/wfclib_utils.c)

add_library(wfclib SHARED ${WFCLIB_SOURCES})
target_include_directories(wfclib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(wfclib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(wfclib PROPERTIES SOVERSION 1)
set_target_properties(wfclib PROPERTIES PUBLIC_HEADER include/wfclib_api.h)

