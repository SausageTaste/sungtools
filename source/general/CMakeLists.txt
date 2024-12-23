add_library(sungtools_general STATIC
    src/angle.cpp
    src/bytes.cpp
    src/cvar.cpp
    src/logic_gate.cpp
    src/stringtool.cpp
    src/threading.cpp
    src/time.cpp
)
add_library(sungtools::general ALIAS sungtools_general)
target_include_directories(sungtools_general PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${extern_dir}/expected/include
    ${extern_dir}/optional/include
)
