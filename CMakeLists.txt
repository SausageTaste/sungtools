cmake_minimum_required(VERSION 3.11)

project(SungTools
    VERSION 0.1.0
    LANGUAGES CXX
)

set(SUNG_ENABLE_TEST OFF CACHE BOOL "Enable testing")

if(SUNG_ENABLE_TEST)
    enable_testing()
endif()


add_subdirectory(source)
