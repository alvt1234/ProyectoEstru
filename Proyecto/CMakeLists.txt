cmake_minimum_required(VERSION 3.10)
project(SDLSimpleApp)

set(CMAKE_CXX_STANDARD 11)

# Encuentra SDL2, SDL2_image y SDL2_ttf
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Incluye los directorios
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Define el ejecutable
add_executable(SDLSimpleApp main.cpp grafo.cpp Semaforos.cpp)

# Vincula las bibliotecas necesarias
target_link_libraries(SDLSimpleApp SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
