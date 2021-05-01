cmake_minimum_required(VERSION 3.19.6)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(RayTracingExample)

add_compile_options(-Wall -pedantic)
set(CMAKE_CXX_STANDARD 17 REQUIRED)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "D:/Moje Wyczyny/Pliki i Repozytoria/SFML/SFML-2.5.1/lib/cmake/SFML")
#set(SFML_DIR "D:/Programy/CodeBlocks/sfml/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

file(GLOB SOURCE "src/*.cpp" "../lib/src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE})

include_directories(include)
include_directories(../lib/include)

target_link_libraries(${PROJECT_NAME} m)
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

# cd build
# cmake .. -G "Unix Makefiles"
# make
# RayTracingExample.exe