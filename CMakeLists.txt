cmake_minimum_required(VERSION 3.10)

project(HelloWorld CXX)

find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
 
add_executable(HelloWorld 
Source/Main.cpp 
Source/Engine.cpp 
Source/Input.cpp 
Source/Log.cpp 
Source/Window.cpp
Source/Render.cpp
Source/Editor.cpp

Include/Engine.h
Include/Input.h
Include/Log.h
Include/Module.h
Include/Window.h
Include/Render.h
Include/Editor.h)

target_include_directories(HelloWorld PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_libraries(HelloWorld PRIVATE
    SDL3::SDL3
    glad::glad
    imgui::imgui
)

add_custom_command(TARGET HelloWorld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
    $<TARGET_FILE_DIR:HelloWorld>/Assets
)