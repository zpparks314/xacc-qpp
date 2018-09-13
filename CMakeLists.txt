project(xacc-qpp LANGUAGES CXX)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
message(STATUS "C++ version ${CXX_STANDARD} configured.")

option(QPP_BUILD_TESTS "Build test programs" ON)

find_package(XACC REQUIRED)

# Make sure all build libraries have reference to their dependents
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${XACC_ROOT}" CACHE PATH "default install path" FORCE )
endif()

set(CppMicroServicesConfig_DIR ${XACC_ROOT}/share/cppmicroservices4/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${XACC_ROOT}/share/xacc")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CppMicroServicesConfig_DIR}")
include(CppMicroServicesConfig)


include_directories(${XACC_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/tpls/qpp/include/)
include_directories(${CMAKE_SOURCE_DIR}/accelerator)
set(EIGEN3_INCLUDE_DIR ${XACC_ROOT}/include/eigen)

link_directories(${XACC_LIBRARY_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/accelerator/tests)
add_subdirectory(${CMAKE_SOURCE_DIR}/accelerator)
