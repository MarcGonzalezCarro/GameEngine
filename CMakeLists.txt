cmake_minimum_required(VERSION 3.10)

project(HelloWorld CXX)

find_package(SDL3 CONFIG REQUIRED)

add_executable(HelloWorld 
Source/Main.cpp 
Source/Engine.cpp 
Source/Input.cpp 
Source/Log.cpp 
Source/Window.cpp

Include/Engine.h
Include/Input.h
Include/Log.h
Include/Module.h
Include/Window.h)

target_include_directories(HelloWorld PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_libraries(HelloWorld PRIVATE SDL3::SDL3)