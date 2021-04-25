# Ray Tracing Algorithm on CPU
Simple ray tracer created for fun and to develop object programing skills.

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

## Workflow
#### Tasks in progress:
- overall optimalization
- torus test implementation
- better documentation

#### To do:
- code division for lib and example usage
- transparent materials and light refraction effect
