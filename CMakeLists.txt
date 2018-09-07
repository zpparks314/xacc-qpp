project(xacc-qpp LANGUAGES CXX)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
message(STATUS "C++ version ${CXX_STANDARD} configured.")

option(QPP_BUILD_TESTS "Build test programs" ON)

find_package(XACC REQUIRED)


if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${XACC_ROOT}" CACHE PATH "default install path" FORCE )
endif()


include_directories(${XACC_INCLUDE_DIRS})
include_directories(/tpls/qpp/include)
set(EIGEN3_INCLUDE_DIR ${XACC_ROOT}/include/eigen)

link_directories(${XACC_LIBRARY_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/accelerator/tests)
add_subdirectory(${CMAKE_SOURCE_DIR}/accelerator)
