cmake_minimum_required (VERSION 2.8)

project(vPipeline)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} /usr/include/ include BasicNetworking/include)

add_executable(vPipeline BasicNetworking/basicNetworking.cpp vPipeline.cpp GripPipeline.cpp)

LINK_DIRECTORIES(/usr/lib)
target_link_libraries(vPipeline ${OpenCV_LIBS} )
add_definitions(-pthread -Wall -std=c++11 -lstdc++)
