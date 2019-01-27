cmake_minimum_required(VERSION 3.2)
set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(PATCH_VERSION 999)
if($ENV{MAJOR_VERSION})
    set(MAJOR_VERSION $ENV{MAJOR_VERSION})
endif()
if($ENV{MINOR_VERSION})
    set(MINOR_VERSION $ENV{MINOR_VERSION})
endif()
if($ENV{TRAVIS_BUILD_NUMBER})
    set(PATCH_VERSION $ENV{TRAVIS_BUILD_NUMBER})
endif()

project(nickname VERSION ${MAJOR_VERSION}.${MAJOR_VERSION}.${PATCH_VERSION})
configure_file(version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_executable(nickname main.cpp)

set_target_properties(nickname PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

target_link_libraries(nickname
    radix
)
install(TARGETS nickname RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT podshivalov.ilya@yandex.ru)
include(CPack)
add_test(nickname_test_version ${CMAKE_CURRENT_BINARY_DIR}/tests/test_version)
add_test(radix_tests ${CMAKE_CURRENT_BINARY_DIR}/tests/test_radix)
enable_testing()
