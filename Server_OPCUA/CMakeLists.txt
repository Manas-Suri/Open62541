cmake_minimum_required(VERSION 3.10)
project(Open62541Example CXX)


# Please do the required changes in the path add_subdirectory()
add_subdirectory(/home/msuri/Downloads/Prev_Laptop/Work/Rpi_Imager/Open62541/open62541 /home/msuri/Downloads/Prev_Laptop/Work/Rpi_Imager/Server_OPCUA/build1)
include_directories(/open62541/include)

add_executable(Open62541Example main.cpp pumpVariables.cpp)
target_link_libraries(Open62541Example open62541)


