cmake_minimum_required(VERSION 3.11)

project(SungTools
    VERSION 0.1.0
    LANGUAGES CXX
)


# Some variables
#-------------------------------------------------------------------------------

set(SUNG_ENABLE_TEST OFF CACHE BOOL "Build test executables")

set(sung_extern_dir ${CMAKE_CURRENT_SOURCE_DIR}/extern)
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


# Define the library
#-------------------------------------------------------------------------------

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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${sung_extern_dir}/expected/include>
    $<BUILD_INTERFACE:${sung_extern_dir}/optional/include>
    $<INSTALL_INTERFACE:include>
    $<INSTALL_INTERFACE:expected/include>
    $<INSTALL_INTERFACE:optional/include>
)


# Installation
#-------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SungToolsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(TARGETS sungtools_basic
    EXPORT SungToolsTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY extern/expected/include/ DESTINATION include)
install(DIRECTORY extern/optional/include/ DESTINATION include)
install(EXPORT SungToolsTargets
    FILE SungToolsTargets.cmake
    NAMESPACE sungtools::
    DESTINATION lib/cmake/SungTools
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SungToolsConfigVersion.cmake"
    DESTINATION lib/cmake/SungTools
)


# Test
#-------------------------------------------------------------------------------

if (SUNG_ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
