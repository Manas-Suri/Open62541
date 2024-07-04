cmake_minimum_required(VERSION 3.10)
project(Open62541Example CXX)

add_subdirectory(/home/INTRANET.SALZ.GMBH/manas.suri/Work/Rpi_Imager/Open62541/open62541 /home/INTRANET.SALZ.GMBH/manas.suri/Work/Rpi_Imager/Server_OPCUA/build1)
include_directories(/open62541/include)

add_executable(Open62541Example main.cpp temperature_change.cpp)
target_link_libraries(Open62541Example open62541)

