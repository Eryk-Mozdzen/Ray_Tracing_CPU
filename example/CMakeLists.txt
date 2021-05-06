cmake_minimum_required(VERSION 3.16)

project(RayTracingExample)

# compiler settings
add_compile_options(-Wall -pedantic)
set(CMAKE_CXX_STANDARD 17)

# SFML subdirectory
add_subdirectory(../external/SFML ../../external/build)
set(SFML_DIR "../external/build/")
set(SFML_STATIC_LIBRARIES TRUE)
#find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_package(SFML 2.5 COMPONENTS system window network REQUIRED)

# code source
file(GLOB SOURCE "src/*.cpp" "../lib/src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC ../lib/include)

# linking
target_link_libraries(${PROJECT_NAME} m)
#target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-network)

# git clone 
# cd RayTracing_CPU/external
# git submodule init SFML
# git submodule update

# mkdir build
# cd build
# cmake -S ../SFML/ -B . -G "Unix Makefiles" -DBUILD_SHARED_LIBS=FALSE -DSFML_USE_STATIC_STD_LIBS=TRUE
# make

# cd ../example
# mkdir build
# cd build
# cmake -S .. -B . -G "Unix Makefiles" -DBUILD_SHARED_LIBS=FALSE -DSFML_USE_STATIC_STD_LIBS=TRUE
# make
# RayTracingExample.exe