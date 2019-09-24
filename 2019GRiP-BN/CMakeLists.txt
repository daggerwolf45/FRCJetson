cmake_minimum_required (VERSION 2.8)

project(vPipeline)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/BasicNetworking/cmake/" FORCE)

find_package(OpenCV REQUIRED)
find_package(FindSQLite3 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${sqlite_INCLUDE_DIRS}$ /usr/include/ include BasicNetworking/include 
BasicNetworking/include/b64pack/include)

add_executable(vPipeline vPipeline.cpp BasicNetworking/basicNetworking.cpp GripPipeline.cpp)

LINK_DIRECTORIES(/usr/lib)
target_link_libraries(vPipeline ${OpenCV_LIBS} ${sqlite_LIBRARIES})
add_definitions(-ldl -pthread -Wall -std=c++11 -lstdc++ )
