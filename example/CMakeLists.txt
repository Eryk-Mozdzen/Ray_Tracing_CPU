cmake_minimum_required(VERSION 3.16)

project(rtrace-example)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(
	-O3
	
	-Wall 
	-pedantic 
)

add_subdirectory(
	"../external/SFML"
	"../../external/build"
)
set(SFML_DIR "../external/build/")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS 
	system 
	window 
	graphics 
	network 
	audio REQUIRED
)

file(GLOB SOURCE "src/*.cpp" "../rtrace/src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC 
	"include"
	"../rtrace/include"
)

target_link_libraries(${PROJECT_NAME} 
	m

	sfml-system
	sfml-window
	sfml-graphics
	sfml-network
	sfml-audio
)

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
# ./rtrace-example