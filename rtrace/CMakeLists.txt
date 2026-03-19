
add_library(rtrace
	src/BoundingBox.cpp
	src/Collision.cpp
	src/Color.cpp
	src/Light.cpp
	src/Material.cpp
	src/Matrix.cpp
	src/Ray.cpp
	src/Scene.cpp
	src/Transform.cpp
	src/Vector.cpp
	src/View.cpp
)

target_include_directories(rtrace PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(rtrace PRIVATE
	m
	tbb
)
