cmake_minimum_required(VERSION 3.16)

project(example-term)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O3 -Wall -pedantic)

add_subdirectory("../rtrace" "rtrace")

add_executable(${PROJECT_NAME}
	"src/main.cpp"
	"src/window.cpp"
	"src/torus.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	"include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	m
	rtrace
	ncurses
)
