# Copyright 2021 Michael Beckh
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" IS_MAIN_PROJECT)
option(BUILD_TESTING "Build tests" ${IS_MAIN_PROJECT})

project(detours-gmock VERSION 1.0.1
    DESCRIPTION "Helper functions and macros for using detours with googlemock."
    HOMEPAGE_URL "https://github.com/mbeckh/detours-gmock"
    LANGUAGES CXX)

find_package(detours REQUIRED)
find_package(GTest REQUIRED)

add_library(gmock INTERFACE
    "include/detours_gmock.h"
)
add_library(detours::gmock ALIAS gmock)

target_compile_features(gmock INTERFACE cxx_std_20)
target_include_directories(gmock INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>" "$<INSTALL_INTERFACE:include>")
set_target_properties(gmock PROPERTIES
    DEBUG_POSTFIX d
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
target_link_libraries(gmock INTERFACE detours::detours GTest::gmock)


include(CMakePackageConfigHelpers)
configure_package_config_file("cmake/detours-gmock-config.cmake.in" "detours-gmock-config.cmake" INSTALL_DESTINATION "share/detours-gmock")
write_basic_package_version_file("${PROJECT_BINARY_DIR}/detours-gmock-config-version.cmake" VERSION ${detours-gmock_VERSION} COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)

install(TARGETS gmock EXPORT detours-gmock-targets)
install(FILES "include/detours_gmock.h" TYPE INCLUDE)
install(EXPORT detours-gmock-targets DESTINATION "share/detours-gmock" NAMESPACE "detours::")
install(FILES "${PROJECT_BINARY_DIR}/detours-gmock-config-version.cmake" "${PROJECT_BINARY_DIR}/detours-gmock-config.cmake" DESTINATION "share/detours-gmock")


if(BUILD_TESTING)
    include(GoogleTest)
    enable_testing()

    add_executable(detours-gmock_Test
        "test/detours_gmock_Test.cpp"
    )

    target_compile_definitions(detours-gmock_Test PRIVATE WIN32_LEAN_AND_MEAN=1 NOMINMAX=1)
    target_compile_features(detours-gmock_Test PRIVATE cxx_std_20)
    target_precompile_headers(detours-gmock_Test PRIVATE "test/pch.h")
    set_target_properties(detours-gmock_Test PROPERTIES
        DEBUG_POSTFIX d
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    target_link_libraries(detours-gmock_Test PRIVATE detours::gmock GTest::gtest GTest::gtest_main shlwapi)

    gtest_discover_tests(detours-gmock_Test)
endif()
