cmake_minimum_required(VERSION 3.10)
project(Open62541Example CXX)

# Add open62541 subdirectory
add_subdirectory(open62541)
include_directories(open62541/include)

add_executable(Open62541Example main.cpp)
target_link_libraries(Open62541Example open62541)

