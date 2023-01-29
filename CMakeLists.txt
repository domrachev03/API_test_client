cmake_minimum_required(VERSION 3.24)
project(pendulum_test_api C)

set(CMAKE_C_STANDARD 11)

find_package(lcm REQUIRED)
include(${LCM_USE_FILE})

lcm_wrap_types(
        C_EXPORT my_lcmtypes
        C_SOURCES c_sources
        C_HEADERS c_headers
        lcm_packages/api_pkg.lcm
        lcm_packages/server_pkg.lcm
        lcm_packages/response_pkg.lcm
        lcm_packages/request_pkg.lcm
)

lcm_add_library(my_lcmtypes C ${c_sources} ${c_headers})
include(GenerateExportHeader)
generate_export_header(my_lcmtypes)
target_include_directories(my_lcmtypes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

include_directories(include)

lcm_install_headers(DESTINATION include
        ${CMAKE_CURRENT_BINARY_DIR}/my_lcmtypes_export.h
        ${c_install_headers})

install(TARGETS my_lcmtypes
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib${LIB_SUFFIX}
        ARCHIVE DESTINATION lib${LIB_SUFFIX}
        INCLUDES DESTINATION include
        )


add_executable(pendulum_test_api src/main.c)

target_link_libraries(pendulum_test_api lcm)
lcm_target_link_libraries(pendulum_test_api my_lcmtypes)
