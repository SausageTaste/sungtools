cmake_minimum_required(VERSION 3.23)

project(sungtools
    VERSION 0.1.1
    LANGUAGES CXX
)


# Some variables
#-------------------------------------------------------------------------------

set(SUNG_ENABLE_TEST OFF CACHE BOOL "Build test executables")

set(sung_extern_dir ${CMAKE_CURRENT_SOURCE_DIR}/extern)
set(sung_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(sung_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)


# Define the library
#-------------------------------------------------------------------------------

set(sung_header_basic
    ${sung_include_dir}/sung/basic/aabb.hpp
    ${sung_include_dir}/sung/basic/angle.hpp
    ${sung_include_dir}/sung/basic/byte_arr.hpp
    ${sung_include_dir}/sung/basic/bytes.hpp
    ${sung_include_dir}/sung/basic/cvar.hpp
    ${sung_include_dir}/sung/basic/densify.hpp
    ${sung_include_dir}/sung/basic/expected.hpp
    ${sung_include_dir}/sung/basic/geometry2d.hpp
    ${sung_include_dir}/sung/basic/geometry3d.hpp
    ${sung_include_dir}/sung/basic/inputs.hpp
    ${sung_include_dir}/sung/basic/linalg.hpp
    ${sung_include_dir}/sung/basic/logic_gate.hpp
    ${sung_include_dir}/sung/basic/mamath.hpp
    ${sung_include_dir}/sung/basic/mesh_builder.hpp
    ${sung_include_dir}/sung/basic/optional.hpp
    ${sung_include_dir}/sung/basic/os_detect.hpp
    ${sung_include_dir}/sung/basic/random.hpp
    ${sung_include_dir}/sung/basic/ratio.hpp
    ${sung_include_dir}/sung/basic/static_arr.hpp
    ${sung_include_dir}/sung/basic/static_pool.hpp
    ${sung_include_dir}/sung/basic/stringtool.hpp
    ${sung_include_dir}/sung/basic/threading.hpp
    ${sung_include_dir}/sung/basic/time.hpp
    ${sung_include_dir}/sung/basic/units.hpp
)

set(sung_src_basic
    ${sung_src_dir}/basic/angle.cpp
    ${sung_src_dir}/basic/byte_arr.cpp
    ${sung_src_dir}/basic/bytes.cpp
    ${sung_src_dir}/basic/cvar.cpp
    ${sung_src_dir}/basic/densify.cpp
    ${sung_src_dir}/basic/geometry3d.cpp
    ${sung_src_dir}/basic/inputs.cpp
    ${sung_src_dir}/basic/logic_gate.cpp
    ${sung_src_dir}/basic/mesh_builder.cpp
    ${sung_src_dir}/basic/stringtool.cpp
    ${sung_src_dir}/basic/threading.cpp
    ${sung_src_dir}/basic/time.cpp
)

add_library(sungtools_basic STATIC)
add_library(sungtools::sungtools_basic ALIAS sungtools_basic)
target_sources(sungtools_basic
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            ${sung_include_dir}
            ${sung_extern_dir}/expected/include
            ${sung_extern_dir}/optional/include
        FILES
            ${sung_header_basic}
            ${sung_extern_dir}/expected/include/tl/expected.hpp
            ${sung_extern_dir}/optional/include/tl/optional.hpp
    PRIVATE
        ${sung_src_basic}
)
target_include_directories(sungtools_basic PUBLIC
    $<BUILD_INTERFACE:${sung_include_dir}>
    $<BUILD_INTERFACE:${sung_extern_dir}/expected/include>
    $<BUILD_INTERFACE:${sung_extern_dir}/optional/include>
    $<INSTALL_INTERFACE:include>
)
set_target_properties(sungtools_basic PROPERTIES
    FOLDER "sungtools"
)


# Installation
#-------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION share/${PROJECT_NAME}
)

install(TARGETS sungtools_basic
    EXPORT ${PROJECT_NAME}Targets
    FILE_SET HEADERS DESTINATION include
    LIBRARY DESTINATION $<IF:$<CONFIG:Debug>,debug/lib,lib>
    ARCHIVE DESTINATION $<IF:$<CONFIG:Debug>,debug/lib,lib>
    RUNTIME DESTINATION $<IF:$<CONFIG:Debug>,debug/bin,bin>
)
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION share/${PROJECT_NAME}
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION share/${PROJECT_NAME}
)


# Test
#-------------------------------------------------------------------------------

if (SUNG_ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
