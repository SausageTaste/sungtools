add_library(sungtools_general STATIC
    src/angle.cpp
)
add_library(sungtools::general ALIAS sungtools_general)
target_include_directories(sungtools_general PUBLIC include)


add_executable(sungtools_test_angle
    test/angle.cpp
)
add_test(sungtools_test_angle sungtools_test_angle)
target_link_libraries(sungtools_test_angle
    sungtools::general
)
