cmake_minimum_required(VERSION 3.10)
project(Open62541Example1 CXX)

add_subdirectory(/home/INTRANET.SALZ.GMBH/manas.suri/Work/Rpi_Imager/Open62541/open62541 /home/INTRANET.SALZ.GMBH/manas.suri/Work/Rpi_Imager/Client_OPCUA/build1)
include_directories(/open62541/include)

add_executable(Open62541Example1 main.cpp)
target_link_libraries(Open62541Example1 open62541)

