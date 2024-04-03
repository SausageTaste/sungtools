add_library(sungtools_general STATIC
    src/angle.cpp
    src/logic_gate.cpp
    src/stringtool.cpp
    src/time.cpp
)
add_library(sungtools::general ALIAS sungtools_general)
target_include_directories(sungtools_general PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${sung_fetch_optional_SOURCE_DIR}/include
    ${sung_fetch_expected_SOURCE_DIR}/include
)
