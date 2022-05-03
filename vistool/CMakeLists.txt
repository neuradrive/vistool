cmake_minimum_required(VERSION 3.10)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(PROJECT_NAME "vistool")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Ofast -Wfatal-errors")

#OpenCV
find_package(OpenCV REQUIRED)

if (YAML_CONF)
  find_package(yaml-cpp REQUIRED)
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIR}
)

message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBRARIES: ${OpenCV_LIBRARIES}")

FILE(GLOB_RECURSE CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE CC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")

add_library(${PROJECT_NAME} SHARED ${CPP_FILES} ${CC_FILES})

target_link_libraries(
  ${PROJECT_NAME}
  ${OpenCV_LIBS}
)

find_package (Threads)

add_executable(vistest test/main.cc)

if (YAML_CONF)
  target_link_libraries(vistest
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    ${YAML_CPP_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
  )
else()
  target_link_libraries(vistest
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
  )
endif()
