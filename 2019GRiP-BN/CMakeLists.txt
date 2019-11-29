cmake_minimum_required (VERSION 2.8)

project(vPipeline)

find_package(OpenCV REQUIRED)

set (LIB_DIR_LOCATION /usr/bin/)
find_library(SQLite3 NAMES sqlite3 PATHS ${LIB_DIR_LOCATION})

include_directories(${OpenCV_INCLUDE_DIRS} SQLite3 /usr/include/ include BasicNetworking/include 
BasicNetworking/include/b64pack/include)

add_executable(vPipeline vPipeline.cpp BasicNetworking/basicNetworking.cpp GripPipeline.cpp)

LINK_DIRECTORIES(/usr/lib)
target_link_libraries(vPipeline ${OpenCV_LIBS} sqlite3 ${SQLite3_LIBRARIES})
add_definitions(-ldl -pthread -Wall -std=c++11 -lstdc++ )
