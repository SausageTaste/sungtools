add_library(sung_math STATIC
    src/angle.cpp
)
add_library(sung::math ALIAS sung_math)
target_include_directories(sung_math PUBLIC include)


add_executable(test_angle
    test/angle.cpp
)
add_test(test_angle test_angle)
target_link_libraries(test_angle
    sung::math
)
