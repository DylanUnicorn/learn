cmake_minimum_required(VERSION 3.28)
project(FormattableObjectExample)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the fmt library (ensure it's installed on your system)
find_package(fmt magic_get REQUIRED)

# Add your source files
add_executable(FormattableObjectExample FormatObject.cpp)

# Link the fmt library to your executable
target_link_libraries(FormattableObjectExample PRIVATE fmt::fmt magic_get::magic_get)

# Include directories for headers
target_include_directories(FormattableObjectExample PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
