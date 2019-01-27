cmake_minimum_required(VERSION 3.2)
add_library(radix SHARED
    lib.cpp
    #radix_tree.cpp
    )
set_target_properties(radix PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)
