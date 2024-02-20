cmake_minimum_required(VERSION 3.11)
project(SungTools
    VERSION 0.1.0
    LANGUAGES CXX
)

include(FetchContent)


set(SUNG_ENABLE_TEST OFF CACHE BOOL "Enable testing")

if(SUNG_ENABLE_TEST)
    enable_testing()
endif()


FetchContent_Declare(sung_fetch_optional
    GIT_REPOSITORY    https://github.com/TartanLlama/optional
    GIT_TAG           v1.1.0
)

FetchContent_Populate(
    sung_fetch_optional
)


add_subdirectory(source)
