cmake_minimum_required(VERSION 3.10)
project(SDLSimpleApp)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)  

add_executable(SDLSimpleApp main.cpp
grafo.cpp Semaforos.cpp)

target_link_libraries(SDLSimpleApp SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image)