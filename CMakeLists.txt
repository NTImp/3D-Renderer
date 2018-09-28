cmake_minimum_required(VERSION 3.1)
project(test)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_FLAGS o3)
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

include_directories(src)

file(GLOB_RECURSE src_files "src/*.cpp")

link_libraries(SDL2 SDL2main SDL2_image)
add_executable(game ${src_files})
