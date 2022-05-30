# From: https://decovar.dev/blog/2021/03/08/cmake-cpp-library/

cmake_minimum_required(VERSION 3.15)

set(namespace "eepromAt24cxx")
project("EepromAT24CXX"
    VERSION 0.0.1
    DESCRIPTION "Library for read write on EEPROMs of type 24CXX via I2C"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/eeprom24cxx.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# without it public headers won't get installed
set(public_headers
    include/eeprom24cxx.hpp
)

target_link_libraries(${PROJECT_NAME}
  i2c
)

# note that ${public_headers} has to be in quotes
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")

# not for MSVC
if(CMAKE_COMPILER_IS_GNUCXX)
    # compile options for this target only
    # treat warnings as errors
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
endif()

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")



if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  #message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
  #message(STATUS "This project is a top-level one")

  include(FetchContent)
  FetchContent_Declare(commander
    GIT_REPOSITORY https://github.com/turbodt/cpp-commander.git
    GIT_TAG main
  )
  FetchContent_MakeAvailable(commander)

  set(CLI_NAME at24cxx)
  add_executable(
    ${CLI_NAME}
    cli/utils.cpp
    cli/main.cpp
    cli/clear.cpp
    cli/read.cpp
    cli/write.cpp
  )
  target_include_directories(${CLI_NAME} PRIVATE cli/include)
  target_link_libraries(${CLI_NAME} PUBLIC ${PROJECT_NAME})
  target_link_libraries(${CLI_NAME} PRIVATE Commander)
endif()


