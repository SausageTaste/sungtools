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

add_executable(sungtest_general_cvar cvar.cpp)
add_test(sungtest_general_cvar sungtest_general_cvar)
target_link_libraries(sungtest_general_cvar ${LIB_GENERAL})

add_executable(sungtest_general_expected expected.cpp)
add_test(sungtest_general_expected sungtest_general_expected)
target_link_libraries(sungtest_general_expected ${LIB_GENERAL})

add_executable(sungtest_general_geometry2d geometry2d.cpp)
add_test(sungtest_general_geometry2d sungtest_general_geometry2d)
target_link_libraries(sungtest_general_geometry2d ${LIB_GENERAL})

add_executable(sungtest_general_geometry3d geometry3d.cpp)
add_test(sungtest_general_geometry3d sungtest_general_geometry3d)
target_link_libraries(sungtest_general_geometry3d ${LIB_GENERAL})

add_executable(sungtest_general_linalg linalg.cpp)
add_test(sungtest_general_linalg sungtest_general_linalg)
target_link_libraries(sungtest_general_linalg ${LIB_GENERAL})

add_executable(sungtest_general_linalg_glm linalg_glm.cpp)
add_test(sungtest_general_linalg_glm sungtest_general_linalg_glm)
target_link_libraries(sungtest_general_linalg_glm ${LIB_GENERAL} glm::glm)

add_executable(sungtest_general_logicgate logic_gate.cpp)
add_test(sungtest_general_logicgate sungtest_general_logicgate)
target_link_libraries(sungtest_general_logicgate ${LIB_GENERAL})

add_executable(sungtest_general_mamath mamath.cpp)
add_test(sungtest_general_mamath sungtest_general_mamath)
target_link_libraries(sungtest_general_mamath ${LIB_GENERAL})

add_executable(sungtest_general_optional optional.cpp)
add_test(sungtest_general_optional sungtest_general_optional)
target_link_libraries(sungtest_general_optional ${LIB_GENERAL})

add_executable(sungtest_general_random random.cpp)
add_test(sungtest_general_random sungtest_general_random)
target_link_libraries(sungtest_general_random ${LIB_GENERAL})

add_executable(sungtest_general_static_arr static_arr.cpp)
add_test(sungtest_general_static_arr sungtest_general_static_arr)
target_link_libraries(sungtest_general_static_arr ${LIB_GENERAL})

add_executable(sungtest_general_stringtool stringtool.cpp)
add_test(sungtest_general_stringtool sungtest_general_stringtool)
target_link_libraries(sungtest_general_stringtool ${LIB_GENERAL})

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

if (sung_cpp20_supported)
    add_executable(sungtest_general_time_20 time.cpp)
    add_test(sungtest_general_time_20 sungtest_general_time_20)
    target_link_libraries(sungtest_general_time_20 ${LIB_GENERAL})
    target_compile_features(sungtest_general_time_20 PUBLIC cxx_std_20)
else()
    message("[SUNG] C++ 20 is not supported so some tests are not created.")
endif()

if (sung_cpp23_supported)
    add_executable(sungtest_general_bytes_23 bytes.cpp)
    add_test(sungtest_general_bytes_23 sungtest_general_bytes_23)
    target_link_libraries(sungtest_general_bytes_23 ${LIB_GENERAL})
    target_compile_features(sungtest_general_bytes_23 PUBLIC cxx_std_23)

    add_executable(sungtest_general_expected_23 expected.cpp)
    add_test(sungtest_general_expected_23 sungtest_general_expected_23)
    target_link_libraries(sungtest_general_expected_23 ${LIB_GENERAL})
    target_compile_features(sungtest_general_expected_23 PUBLIC cxx_std_23)
else()
    message("[SUNG] C++ 23 is not supported so some tests are not created.")
endif()
