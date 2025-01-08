add_library(sungtools_gameengine STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data_central.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/inputs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh_builder.cpp
)
add_library(sungtools::gameengine ALIAS sungtools_gameengine)
target_include_directories(sungtools_gameengine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(sungtools_gameengine PUBLIC sungtools::general)
