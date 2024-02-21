add_library(sungtools_general STATIC
    src/angle.cpp
    src/logic_gate.cpp
    src/time.cpp
)
add_library(sungtools::general ALIAS sungtools_general)
target_include_directories(sungtools_general PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${sung_fetch_optional_SOURCE_DIR}/include
    ${sung_fetch_expected_SOURCE_DIR}/include
)


if (SUNG_ENABLE_TEST)
    add_executable(sungtools_test_aabb test/aabb.cpp)
    add_test(sungtools_test_aabb sungtools_test_aabb)
    target_link_libraries(sungtools_test_aabb sungtools::general)

    add_executable(sungtools_test_angle test/angle.cpp)
    add_test(sungtools_test_angle sungtools_test_angle)
    target_link_libraries(sungtools_test_angle sungtools::general)

    add_executable(sungtools_test_expected test/expected.cpp)
    add_test(sungtools_test_expected sungtools_test_expected)
    target_link_libraries(sungtools_test_expected sungtools::general)

    add_executable(sungtools_test_expected_23 test/expected.cpp)
    add_test(sungtools_test_expected_23 sungtools_test_expected_23)
    target_link_libraries(sungtools_test_expected_23 sungtools::general)
    target_compile_features(sungtools_test_expected_23 PUBLIC cxx_std_23)

    add_executable(sungtools_test_logicgate test/logic_gate.cpp)
    add_test(sungtools_test_logicgate sungtools_test_logicgate)
    target_link_libraries(sungtools_test_logicgate sungtools::general)

    add_executable(sungtools_test_optional test/optional.cpp)
    add_test(sungtools_test_optional sungtools_test_optional)
    target_link_libraries(sungtools_test_optional sungtools::general)

    add_executable(sungtools_test_optional_17 test/optional.cpp)
    add_test(sungtools_test_optional_17 sungtools_test_optional_17)
    target_link_libraries(sungtools_test_optional_17 sungtools::general)
    target_compile_features(sungtools_test_optional_17 PUBLIC cxx_std_17)

    add_executable(sungtools_test_time test/time.cpp)
    add_test(sungtools_test_time sungtools_test_time)
    target_link_libraries(sungtools_test_time sungtools::general)

    add_executable(sungtools_test_units test/units.cpp)
    add_test(sungtools_test_units sungtools_test_units)
    target_link_libraries(sungtools_test_units sungtools::general)

    add_executable(sungtools_test_aabb_units test/aabb_units.cpp)
    add_test(sungtools_test_aabb_units sungtools_test_aabb_units)
    target_link_libraries(sungtools_test_aabb_units sungtools::general)
endif()
