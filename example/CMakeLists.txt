cmake_minimum_required(VERSION 3.16)

project(example-ghx)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O3 -Wall -pedantic)

add_subdirectory("../external/SFML" "SFML")
add_subdirectory("../rtrace" "rtrace")

add_executable(${PROJECT_NAME}
	"src/main.cpp"
	"src/window.cpp"
	"src/camera.cpp"
	"src/sphere.cpp"
	"src/plane.cpp"
	"src/torus.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	"include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	m
	rtrace
	sfml-system
	sfml-window
	sfml-graphics
	sfml-network
	sfml-audio
)
