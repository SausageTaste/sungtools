add_library(sungtools_general STATIC
    src/angle.cpp
    src/logic_gate.cpp
    src/time.cpp
)
add_library(sungtools::general ALIAS sungtools_general)
target_include_directories(sungtools_general PUBLIC include)


if (SUNG_ENABLE_TEST)
    add_executable(sungtools_test_angle test/angle.cpp)
    add_test(sungtools_test_angle sungtools_test_angle)
    target_link_libraries(sungtools_test_angle sungtools::general)

    add_executable(sungtools_test_time test/time.cpp)
    add_test(sungtools_test_time sungtools_test_time)
    target_link_libraries(sungtools_test_time sungtools::general)

    add_executable(sungtools_test_logicgate test/logic_gate.cpp)
    add_test(sungtools_test_logicgate sungtools_test_logicgate)
    target_link_libraries(sungtools_test_logicgate sungtools::general)
endif()
