# Ray Tracing Algorithm on CPU
Simple ray tracer created for fun and to develop object programing skills.
Code uses SFML library to display result on `sf::RenderWindow` and menage basic objects like `sf::Color` and `sf::Image`

YouTube demonstration: https://www.youtube.com/watch?v=HcqXAsXDC6k

![](/saved_frame.jpg)

## Usage
In order to render a frame you need:

#### Custom drawable object
Lib does not have any pre-made classes to draw on scene. User must declare his own objects inheriting from `Object` class and overrite
`bool intersect(const Ray &ray, const CollisionData &data) const` method on it.

`Ray` struct have two fields:
- `.origin` type `Vector3` (point in 3d space, begin of the ray)
- `.direction` type `Vector3` (normalized vector where ray is pointing)

Method must return:
- `false` if `Ray` struct does not collide with user's custom drawable object
- `true` if collide. 

Also user must fill fields of `CollisionData` struct in correct way:
- `.point` type `Vector3` (should contain coordinates of point in 3d space where collision occurs)
- `.normal` type `Vector3` (should contain surface norma vector in collision point)
- `.color` type `sf::Color` (should contain color of the surface in collision point)
- `.material` type `Material` (should contain material specifications of the object)
- `.distance` type `double` (should contain distance dorm ray origin to collision point)
- `.exist` type `bool` (should contain result of intersection test, exacly the same what method return)

### Camera view
`View` class describes position and direction of "virtual camera" on scene. It also contains distance from eye to trajectory plane.

### RenderScene class
`RenderScene` class is the core of algorithm. It contains informations about all objects and lights on screen. Class have several metods like:
- `void addObject(Object*)` adds pointer to `Object` to scene
- `void addLightSource(LightSource*)` add pointer to `LightSource` to scene
- `void setReflectionDepth(const unsigned int&)` set number of ray bounces from objects in scene.
- `void setRenderResolution(const unsigned int&, const unsigned int&)` set resolution of render image (one ray for pixel + reflection depth).
- `Object* getObjectReference(const unsigned int&)` returns pointer to object.
- `const unsigned int & getReflectionDepth() const` return reflection depth.
- `const unsigned int & getRenderResolutionWidth() const` returns render width.
- `const unsigned int & getRenderResolutionHeight() const` returns render height.
- `sf::Image render(const View&) const` the moset important method. Returns rendered image of scnene from specific view.

## Workflow
#### Tasks in progress:
- overall optimalization
- torus test implementation
- better documentation

#### To do:
- code division for lib and example usage
- transparent materials and light refraction effect
