cmake_minimum_required(VERSION 3.20)

project(nithcraft)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 20)

set(MAIN ${PROJECT_NAME})

file(GLOB_RECURSE NITH_SOURCES "src/*.cpp")

add_executable(${MAIN} ${NITH_SOURCES})

target_include_directories(${MAIN} PUBLIC src)

target_precompile_headers(${MAIN} PUBLIC src/nithpch.hpp)

add_subdirectory(third_party/glad)
target_link_libraries(${MAIN} PUBLIC glad)

find_package(robin_hood CONFIG REQUIRED)
target_link_libraries(${MAIN} PUBLIC robin_hood::robin_hood)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${MAIN} PUBLIC glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${MAIN} PUBLIC glm::glm)

find_package(OpenGL REQUIRED)
target_link_libraries(${MAIN} PUBLIC OpenGL::GL)

add_subdirectory(third_party/nanobench)
target_link_libraries(${MAIN} PUBLIC nanobench)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(${MAIN} PRIVATE EnTT::EnTT)