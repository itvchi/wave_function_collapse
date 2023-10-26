cmake_minimum_required(VERSION 3.13)

project(wfc C)

set(WFC_SOURCES
    src/main.c)

set(SDL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdllib/include)
set(SDL_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/sdllib/lib)

add_executable(wfc ${WFC_SOURCES})
target_compile_options(wfc PRIVATE -Wall)
target_include_directories(wfc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(wfc PRIVATE ${SDL_INCLUDE_DIR})
target_link_directories(wfc PRIVATE ${SDL_LIBRARY})
target_link_libraries(wfc SDL3)

# add wfclib to compile and link with the main project
add_subdirectory(wfclib)
set(WFCLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wfclib/include)
set(WFCLIB_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/build/wfclib)
target_include_directories(wfc PRIVATE ${WFCLIB_INCLUDE_DIR})
target_link_directories(wfc PRIVATE ${WFCLIB_LIBRARY})
target_link_libraries(wfc wfclib)
