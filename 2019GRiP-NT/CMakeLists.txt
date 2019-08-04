cmake_minimum_required (VERSION 2.8)

project(ic_pipeline)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} /usr/include/ network_tables/include/)

add_executable(ic_pipeline ic_pipeline.cpp GripPipeline.cpp network_tables/cpp/networktables/NetworkTable.cpp network_tables/cpp/networktables/NetworkTableEntry.cpp network_tables/cpp/networktables/NetworkTableInstance.cpp)

LINK_DIRECTORIES(~/FRCJetson/2019GRiP-NT/network_tables/Linux/arm/ntcore /usr/lib)
target_link_libraries(ic_pipeline ${OpenCV_LIBS} ~/FRCJetson/2019GRiP-NT/network_tables/Linux/arm/libntcore.so)
add_definitions(-pthread -Wall -std=c++11 -lstdc++ -lntcore -lwpiutil -lcscore)
