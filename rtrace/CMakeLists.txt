
add_library(rtrace
	src/collision.cpp
	src/color.cpp
	src/light.cpp
	src/material.cpp
	src/matrix.cpp
	src/object.cpp
	src/ray.cpp
	src/scene.cpp
	src/tools.cpp
	src/transform.cpp
	src/vector.cpp
	src/view.cpp
)

target_include_directories(rtrace PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(rtrace PRIVATE m)