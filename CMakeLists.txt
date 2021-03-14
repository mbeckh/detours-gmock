cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(detours-gmock VERSION 1.0.0
    DESCRIPTION "Helper functions and macros for using detours with googlemock."
    HOMEPAGE_URL "https://github.com/mbeckh/detours-gmock"
    LANGUAGES CXX)

find_package(detours REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(gmock INTERFACE
    include/detours_gmock.h)

add_library(detours::gmock ALIAS gmock)

target_compile_features(gmock INTERFACE cxx_std_14)

target_include_directories(gmock
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>" "$<INSTALL_INTERFACE:include>")

target_link_libraries(gmock INTERFACE detours::detours GTest::gmock)


include(CMakePackageConfigHelpers)

configure_package_config_file("cmake/detours-gmock-config.cmake.in" "detours-gmock-config.cmake"
    INSTALL_DESTINATION "share/detours-gmock")

write_basic_package_version_file("detours-gmock-config-version.cmake"
    VERSION 1.0.0 COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)


install(TARGETS gmock
        EXPORT detours-gmock-targets)

install(FILES "include/detours_gmock.h"
        TYPE INCLUDE)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/detours-gmock-config-version.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/detours-gmock-config.cmake"
        DESTINATION "share/detours-gmock")

install(EXPORT detours-gmock-targets
        DESTINATION "share/detours-gmock" NAMESPACE "detours::")
