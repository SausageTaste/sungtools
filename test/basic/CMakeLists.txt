set(sungtest_lib_basic GTest::gtest_main sungtools::sungtools_basic)


add_executable(sungtest_basic_aabb_units aabb_units.cpp)
add_test(sungtest_basic_aabb_units sungtest_basic_aabb_units)
target_link_libraries(sungtest_basic_aabb_units ${sungtest_lib_basic})

add_executable(sungtest_basic_aabb aabb.cpp)
add_test(sungtest_basic_aabb sungtest_basic_aabb)
target_link_libraries(sungtest_basic_aabb ${sungtest_lib_basic})

add_executable(sungtest_basic_angle angle.cpp)
add_test(sungtest_basic_angle sungtest_basic_angle)
target_link_libraries(sungtest_basic_angle ${sungtest_lib_basic})

add_executable(sungtest_basic_byte_arr byte_arr.cpp)
add_test(sungtest_basic_byte_arr sungtest_basic_byte_arr)
target_link_libraries(sungtest_basic_byte_arr ${sungtest_lib_basic})

add_executable(sungtest_basic_cvar cvar.cpp)
add_test(sungtest_basic_cvar sungtest_basic_cvar)
target_link_libraries(sungtest_basic_cvar ${sungtest_lib_basic})

add_executable(sungtest_basic_densify densify.cpp)
add_test(sungtest_basic_densify sungtest_basic_densify)
target_link_libraries(sungtest_basic_densify ${sungtest_lib_basic})

add_executable(sungtest_basic_expected expected.cpp)
add_test(sungtest_basic_expected sungtest_basic_expected)
target_link_libraries(sungtest_basic_expected ${sungtest_lib_basic})

add_executable(sungtest_basic_geometry2d geometry2d.cpp)
add_test(sungtest_basic_geometry2d sungtest_basic_geometry2d)
target_link_libraries(sungtest_basic_geometry2d ${sungtest_lib_basic})

add_executable(sungtest_basic_geometry3d geometry3d.cpp)
add_test(sungtest_basic_geometry3d sungtest_basic_geometry3d)
target_link_libraries(sungtest_basic_geometry3d ${sungtest_lib_basic})

add_executable(sungtest_basic_linalg linalg.cpp)
add_test(sungtest_basic_linalg sungtest_basic_linalg)
target_link_libraries(sungtest_basic_linalg ${sungtest_lib_basic})

add_executable(sungtest_basic_linalg_glm linalg_glm.cpp)
add_test(sungtest_basic_linalg_glm sungtest_basic_linalg_glm)
target_link_libraries(sungtest_basic_linalg_glm ${sungtest_lib_basic} glm::glm)

add_executable(sungtest_basic_logicgate logic_gate.cpp)
add_test(sungtest_basic_logicgate sungtest_basic_logicgate)
target_link_libraries(sungtest_basic_logicgate ${sungtest_lib_basic})

add_executable(sungtest_basic_mamath mamath.cpp)
add_test(sungtest_basic_mamath sungtest_basic_mamath)
target_link_libraries(sungtest_basic_mamath ${sungtest_lib_basic})

add_executable(sungtest_basic_optional optional.cpp)
add_test(sungtest_basic_optional sungtest_basic_optional)
target_link_libraries(sungtest_basic_optional ${sungtest_lib_basic})

add_executable(sungtest_basic_random random.cpp)
add_test(sungtest_basic_random sungtest_basic_random)
target_link_libraries(sungtest_basic_random ${sungtest_lib_basic})

add_executable(sungtest_basic_static_arr static_arr.cpp)
add_test(sungtest_basic_static_arr sungtest_basic_static_arr)
target_link_libraries(sungtest_basic_static_arr ${sungtest_lib_basic})

add_executable(sungtest_basic_stringtool stringtool.cpp)
add_test(sungtest_basic_stringtool sungtest_basic_stringtool)
target_link_libraries(sungtest_basic_stringtool ${sungtest_lib_basic})

add_executable(sungtest_basic_threading threading.cpp)
add_test(sungtest_basic_threading sungtest_basic_threading)
target_link_libraries(sungtest_basic_threading ${sungtest_lib_basic})

add_executable(sungtest_basic_time time.cpp)
add_test(sungtest_basic_time sungtest_basic_time)
target_link_libraries(sungtest_basic_time ${sungtest_lib_basic})

add_executable(sungtest_basic_units units.cpp)
add_test(sungtest_basic_units sungtest_basic_units)
target_link_libraries(sungtest_basic_units ${sungtest_lib_basic})

if (sung_cpp17_supported)
    add_executable(sungtest_basic_optional_17 optional.cpp)
    add_test(sungtest_basic_optional_17 sungtest_basic_optional_17)
    target_link_libraries(sungtest_basic_optional_17 ${sungtest_lib_basic})
    target_compile_features(sungtest_basic_optional_17 PUBLIC cxx_std_17)
else()
    message("[SUNG] C++ 17 is not supported so some tests are not created.")
endif()

if (sung_cpp20_supported)
    add_executable(sungtest_basic_time_20 time.cpp)
    add_test(sungtest_basic_time_20 sungtest_basic_time_20)
    target_link_libraries(sungtest_basic_time_20 ${sungtest_lib_basic})
    target_compile_features(sungtest_basic_time_20 PUBLIC cxx_std_20)
else()
    message("[SUNG] C++ 20 is not supported so some tests are not created.")
endif()

if (sung_cpp23_supported)
    add_executable(sungtest_basic_bytes_23 bytes.cpp)
    add_test(sungtest_basic_bytes_23 sungtest_basic_bytes_23)
    target_link_libraries(sungtest_basic_bytes_23 ${sungtest_lib_basic})
    target_compile_features(sungtest_basic_bytes_23 PUBLIC cxx_std_23)

    add_executable(sungtest_basic_expected_23 expected.cpp)
    add_test(sungtest_basic_expected_23 sungtest_basic_expected_23)
    target_link_libraries(sungtest_basic_expected_23 ${sungtest_lib_basic})
    target_compile_features(sungtest_basic_expected_23 PUBLIC cxx_std_23)
else()
    message("[SUNG] C++ 23 is not supported so some tests are not created.")
endif()
