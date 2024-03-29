# Ray Tracing & Ray Marching Algorithm on CPU
Simple ray tracer created for fun and to develop object programing skills.
Code uses SFML library to menage basic objects like `sf::RenderWindow`, `sf::Color` and `sf::Image`

YouTube demonstration:

<p align="center">
  <a href="https://www.youtube.com/watch?v=VN4jbGkxlCg">
    <img src="/docs/readme/yt2_photo_button.jpg">
  </a>
</p>

Ray Tracing result (left) and ray marching result (right).

<p align="center">
  <img src="/docs/readme/ray_tracing.jpg" width="350" height="250" />
  <img src="/docs/readme/ray_marching.jpg" width="350" height="250" />
</p>

## Usage

Windows:
```
git clone https://github.com/Eryk-Mozdzen/Ray_Tracing_CPU.git
cd Ray_Tracing_CPU/example-ghx
git submodule init SFML
git submodule update

mkdir build
cd build
cmake .. -G "Unix Makefiles"
make
example-ghx.exe
```
Linux:
```
sudo apt install libudev-dev
sudo apt install libopenal-dev
sudo apt install libvorbis-dev
sudo apt install libflac-dev
sudo apt install libgl1-mesa-dev
sudo apt install xorg-dev

git clone https://github.com/Eryk-Mozdzen/Ray_Tracing_CPU.git
cd ./Ray_Tracing_CPU/example-ghx
git submodule init SFML
git submodule update

mkdir ./build
cd ./build
cmake .. 
make
./example-ghx
```
Example usage of all functions is presented in code with comments, but for short explanation:

#### Custom drawable object
Lib does not have any pre-made classes to draw on scene. User must declare his own objects inheriting from `Object` class and overrite
`Collision intersect(const Ray &ray) const` or `Collision distance(const Vector3 &point)` method on it.

Method must return `Collision` struct wih correct values in each field.

`Collision` struct contains:
- `.point` type `Vector3` (should contain coordinates of point in 3d space where collision occurs)
- `.normal` type `Vector3` (should contain surface norma vector in collision point)
- `.material` type `Material` (should contain material specifications of the object)
- `.distance` type `double` (should contain distance dorm ray origin to collision point)
- `.exist` type `bool` (should contain result of intersection test, exacly the same what method return)

### Classes
`Material` class describes character of given material in context to Phong reflection model.

`Transform3` class is standard transformation matrix 4x4 to describe translation and rotation in 3d space.

`View` class describes position and direction of "virtual camera" on scene. It also contains distance from eye to trajectory plane.

`Scene` class is the core of algorithm. It contains informations about all objects and lights on screen.

## To do:
- overall optimalization
- transparent materials and light refraction effect
- use of `algorithm`