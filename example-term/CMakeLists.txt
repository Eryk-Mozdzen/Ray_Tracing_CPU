cmake_minimum_required(VERSION 3.16)

project(example-term)

add_compile_options(
	-Wall
	-Wextra
	-Wpedantic
	-O3
	-std=c++23
)

add_subdirectory("../rtrace" "rtrace")

add_executable(${PROJECT_NAME}
	"src/main.cpp"
	"src/Window.cpp"
	"src/Torus.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE
	"include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	m
	rtrace
	ncurses
)
