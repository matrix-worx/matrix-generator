cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
project(mgen CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(project_sources
    Common.hpp
    main.cpp
)

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
if(WIN32)
find_package(Boost
    REQUIRED
    COMPONENTS system program_options date_time chrono
)
else()
find_package(Boost
    REQUIRED
    COMPONENTS system program_options
)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
  "Choose the type of build: Debug/Release" FORCE)
endif()

message(STATUS "Build type is: ${CMAKE_BUILD_TYPE}")

add_subdirectory(libmatrix ${CMAKE_BINARY_DIR}/libmatrix/ EXCLUDE_FROM_ALL)

include_directories(gsl
                    ${CMAKE_BINARY_DIR}/gsl/)

include_directories(libmatrix
                    ${CMAKE_BINARY_DIR}/libmatrix/)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(mgen
               ${project_sources}
)
target_link_libraries(mgen
    matrix
    ${Boost_LIBRARIES}
)
