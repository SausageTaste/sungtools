set(LIB_GENERAL GTest::gtest_main sungtools::general)


add_executable(sungtest_general_aabb_units aabb_units.cpp)
add_test(sungtest_general_aabb_units sungtest_general_aabb_units)
target_link_libraries(sungtest_general_aabb_units ${LIB_GENERAL})

add_executable(sungtest_general_aabb aabb.cpp)
add_test(sungtest_general_aabb sungtest_general_aabb)
target_link_libraries(sungtest_general_aabb ${LIB_GENERAL})

add_executable(sungtest_general_angle angle.cpp)
add_test(sungtest_general_angle sungtest_general_angle)
target_link_libraries(sungtest_general_angle ${LIB_GENERAL})

add_executable(sungtest_general_expected expected.cpp)
add_test(sungtest_general_expected sungtest_general_expected)
target_link_libraries(sungtest_general_expected ${LIB_GENERAL})

add_executable(sungtest_general_linalg linalg.cpp)
add_test(sungtest_general_linalg sungtest_general_linalg)
target_link_libraries(sungtest_general_linalg ${LIB_GENERAL})

add_executable(sungtest_general_logicgate logic_gate.cpp)
add_test(sungtest_general_logicgate sungtest_general_logicgate)
target_link_libraries(sungtest_general_logicgate ${LIB_GENERAL})

add_executable(sungtest_general_optional optional.cpp)
add_test(sungtest_general_optional sungtest_general_optional)
target_link_libraries(sungtest_general_optional ${LIB_GENERAL})

add_executable(sungtest_general_random random.cpp)
add_test(sungtest_general_random sungtest_general_random)
target_link_libraries(sungtest_general_random ${LIB_GENERAL})

add_executable(sungtest_general_time time.cpp)
add_test(sungtest_general_time sungtest_general_time)
target_link_libraries(sungtest_general_time ${LIB_GENERAL})

add_executable(sungtest_general_units units.cpp)
add_test(sungtest_general_units sungtest_general_units)
target_link_libraries(sungtest_general_units ${LIB_GENERAL})

if (sung_cpp17_supported)
    add_executable(sungtest_general_optional_17 optional.cpp)
    add_test(sungtest_general_optional_17 sungtest_general_optional_17)
    target_link_libraries(sungtest_general_optional_17 ${LIB_GENERAL})
    target_compile_features(sungtest_general_optional_17 PUBLIC cxx_std_17)
else()
    message("[SUNG] C++ 17 is not supported so some tests are not created.")
endif()

if (sung_cpp23_supported)
    add_executable(sungtest_general_expected_23 expected.cpp)
    add_test(sungtest_general_expected_23 sungtest_general_expected_23)
    target_link_libraries(sungtest_general_expected_23 ${LIB_GENERAL})
    target_compile_features(sungtest_general_expected_23 PUBLIC cxx_std_23)
else()
    message("[SUNG] C++ 23 is not supported so some tests are not created.")
endif()
