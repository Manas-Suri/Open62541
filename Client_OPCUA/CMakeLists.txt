cmake_minimum_required(VERSION 3.10)
project(Open62541Example1 CXX)

# Set the relative path to the Open62541 source directory
set(OPEN62541_SOURCE_DIR ../open62541)  # Adjust this relative path as needed

# Add Open62541 as a subdirectory if it hasn't been built yet
if(NOT EXISTS "${OPEN62541_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "Open62541 source directory does not exist: ${OPEN62541_SOURCE_DIR}")
endif()

# Add the Open62541 subdirectory
add_subdirectory(${OPEN62541_SOURCE_DIR} ${CMAKE_BINARY_DIR}/open62541)

# Include the Open62541 headers
include_directories(${OPEN62541_SOURCE_DIR}/include)

# Add your executable
add_executable(Open62541Example1 main.cpp libraries/retreiveValues.cpp libraries/saveValues.cpp libraries)

# Link the Open62541 library to your executable
target_link_libraries(Open62541Example1 open62541)

