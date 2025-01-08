set(LIB_GAMEENGINE GTest::gtest_main sungtools::gameengine)


add_executable(sungtest_gameengine_datacen data_central.cpp)
add_test(sungtest_gameengine_datacen sungtest_gameengine_datacen)
target_link_libraries(sungtest_gameengine_datacen ${LIB_GAMEENGINE})
