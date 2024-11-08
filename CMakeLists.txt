cmake_minimum_required(VERSION 3.11)
project(SungTools
    VERSION 0.1.0
    LANGUAGES CXX
)

include(FetchContent)


set(extern_dir ${CMAKE_CURRENT_SOURCE_DIR}/extern)

set(SUNG_ENABLE_TEST OFF CACHE BOOL "Enable testing")


if (cxx_std_17 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    set(sung_cpp17_supported ON)
else()
    set(sung_cpp17_supported OFF)
endif()

if (cxx_std_20 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    set(sung_cpp20_supported ON)
else()
    set(sung_cpp20_supported OFF)
endif()

if (cxx_std_23 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    set(sung_cpp23_supported ON)
else()
    set(sung_cpp23_supported OFF)
endif()


add_subdirectory(source)


if (SUNG_ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
