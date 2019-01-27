cmake_minimum_required(VERSION 3.2)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(test_version test_version.cpp)
set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)
target_link_libraries(test_version
    ${Boost_LIBRARIES}
    radix
)

set_target_properties(test_version PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

