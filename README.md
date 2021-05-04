# Ray Tracing & Ray Marching Algorithm on CPU
Simple ray tracer created for fun and to develop object programing skills.
Code uses SFML library to menage basic objects like `sf::RenderWindow`, `sf::Color` and `sf::Image`

YouTube demonstration:

<p align="center">
  <a href="https://www.youtube.com/watch?v=HcqXAsXDC6k">
    <img src="/readmeStuff/yt2_photo_button.jpg">
  </a>
</p>

Ray Tracing result (left) and ray marching result (right).

<p align="center">
  <img src="/readmeStuff/ray_tracing.jpg" width="470" height="300" />   <img src="/readmeStuff/ray_marching.jpg" width="470" height="300" />
</p>

## Usage
You must change `CMakeLists.txt` file according to SFML path in your system. When you do that you can clone repo by typing:
Windows:
```
git clone https://github.com/Eryk-Mozdzen/Ray_Tracing_CPU.git
cd example/build
cmake .. -G "Unix Makefiles"
make
RayTracingExample.exe
```
Linux:
```
git clone https://github.com/Eryk-Mozdzen/Ray_Tracing_CPU.git
cd ./example/build
cmake ..
make
./RayTracingExample
```
Example usage of all functions is presented in code with comments, but for short explanation:

#### Custom drawable object
Lib does not have any pre-made classes to draw on scene. User must declare his own objects inheriting from `Object` class and overrite
`CollisionData intersect(const Ray &ray) const` or `CollisionData distance(const Vector3 &point)` method on it.

Method must return `CollisionData` struct wih correct values in each field.

`CollisionData` struct contains:
- `.point` type `Vector3` (should contain coordinates of point in 3d space where collision occurs)
- `.normal` type `Vector3` (should contain surface norma vector in collision point)
- `.color` type `sf::Color` (should contain color of the surface in collision point)
- `.material` type `Material` (should contain material specifications of the object)
- `.distance` type `double` (should contain distance dorm ray origin to collision point)
- `.exist` type `bool` (should contain result of intersection test, exacly the same what method return)

### Classes
`Material` class describes character of given material in context to Phong reflection model.

`Transform3` class is standard transformation matrix 4x4 to describe translation and rotation in 3d space.

`View` class describes position and direction of "virtual camera" on scene. It also contains distance from eye to trajectory plane.

`RenderScene` class is the core of algorithm. It contains informations about all objects and lights on screen.

## Workflow
#### Tasks in progress:
- overall optimalization

#### To do:
- transparent materials and light refraction effect
