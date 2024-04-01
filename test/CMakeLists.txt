FetchContent_Declare(
    sung_fetch_googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(sung_fetch_googletest)


add_subdirectory(general)
