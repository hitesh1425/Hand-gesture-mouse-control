cmake_minimum_required(VERSION 2.8.12)

project(demo)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${X11_INCLUDE_DIRS})
link_directories(/usr/lib) 

SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})


add_executable(demo face.cpp)
target_link_libraries(demo -lpthread -lXtst -lm ${OpenCV_LIBS})

