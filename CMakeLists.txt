cmake_minimum_required(VERSION 3.11)

project(SungTools
    VERSION 0.1.0
    LANGUAGES CXX
)


set(SUNG_ENABLE_TEST OFF CACHE BOOL "Build test executables")

set(sung_extern_dir ${CMAKE_CURRENT_SOURCE_DIR}/extern)
set(sung_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(sung_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

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


set(sung_src_basic
    ${sung_src_dir}/basic/angle.cpp
    ${sung_src_dir}/basic/bytes.cpp
    ${sung_src_dir}/basic/cvar.cpp
    ${sung_src_dir}/basic/geometry3d.cpp
    ${sung_src_dir}/basic/inputs.cpp
    ${sung_src_dir}/basic/logic_gate.cpp
    ${sung_src_dir}/basic/mesh_builder.cpp
    ${sung_src_dir}/basic/stringtool.cpp
    ${sung_src_dir}/basic/threading.cpp
    ${sung_src_dir}/basic/time.cpp
)

add_library(sungtools_basic STATIC ${sung_src_basic})
add_library(sungtools::basic ALIAS sungtools_basic)
target_include_directories(sungtools_basic PUBLIC
    ${sung_include_dir}
    ${sung_extern_dir}/expected/include
    ${sung_extern_dir}/optional/include
)


if (SUNG_ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
