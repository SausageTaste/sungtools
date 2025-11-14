include(FetchContent)


FetchContent_Declare(
    sung_fetch_googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    sung_fetch_glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG        1.0.1
)


FetchContent_MakeAvailable(
    sung_fetch_googletest
    sung_fetch_glm
)


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


add_subdirectory(basic)
