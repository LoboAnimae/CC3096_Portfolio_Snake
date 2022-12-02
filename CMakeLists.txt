cmake_minimum_required(VERSION 3.23)
project(Portfolio)

set(CMAKE_CXX_STANDARD 23)

add_executable(Portfolio src/main.cpp src/Game/Game.cpp src/Game/Game.hpp src/Game/Systems.hpp src/Game/Scene.hpp src/Game/Entities.hpp src/Game/Components.hpp src/Game/Scene.cpp src/Game/SystemImpl.hpp)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2 box2d)
