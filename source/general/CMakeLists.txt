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
    sungtools_add_test(aabb_units test/aabb_units.cpp)
    sungtools_add_test(aabb test/aabb.cpp)
    sungtools_add_test(angle test/angle.cpp)
    sungtools_add_test(expected test/expected.cpp)
    sungtools_add_test(linalg test/linalg.cpp)
    sungtools_add_test(logicgate test/logic_gate.cpp)
    sungtools_add_test(optional test/optional.cpp)
    sungtools_add_test(random test/random.cpp)
    sungtools_add_test(time test/time.cpp)
    sungtools_add_test(units test/units.cpp)

    if (sung_cpp17_supported)
        sungtools_add_test(optional_17 test/optional.cpp)
        target_compile_features(sungtest_optional_17 PUBLIC cxx_std_17)
    else()
        message("[SUNG] C++ 17 is not supported so 'sungtest_optional_17' is not created.")
    endif()

    if (sung_cpp23_supported)
        sungtools_add_test(expected_23 test/expected.cpp)
        target_compile_features(sungtest_expected_23 PUBLIC cxx_std_23)
    else()
        message("[SUNG] C++ 23 is not supported so 'sungtest_expected_23' is not created.")
    endif()
endif()
