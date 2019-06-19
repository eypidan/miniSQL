cmake_minimum_required(VERSION 3.0)


project(MiniSQL)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB source_files src/*.cpp src/*/*.cpp )


add_executable(MiniSQL ${source_files})