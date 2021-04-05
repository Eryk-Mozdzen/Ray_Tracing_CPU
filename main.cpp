#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Vector.h"
#include "Matrix.h"
#include "geometry.h"

//#define RESOLUTION_144P
//#define RESOLUTION_240P
//#define RESOLUTION_360P
//#define RESOLUTION_480P
//#define RESOLUTION_720P

#define EPSILON             0.0001
#define WINDOW_WIDTH        1280
#define WINDOW_HEIGHT       720
#define LINEAR_VELOCITY     1.5
#define ANGULAR_VELOCITY    0.1
#define REFLECTION_DEPTH    3

constexpr auto M_PI = 3.14159265358979323846;

//////////////////////////////////////////////////////

#if defined RESOLUTION_144P
    #define RESOLUTION_H        256
    #define RESOLUTION_V        144
#elif defined RESOLUTION_240P
    #define RESOLUTION_H        352
    #define RESOLUTION_V        240
#elif defined RESOLUTION_720P
    #define RESOLUTION_H        1280
    #define RESOLUTION_V        720
#elif defined RESOLUTION_360P
    #define RESOLUTION_H        480
    #define RESOLUTION_V        360
#elif defined RESOLUTION_480P
    #define RESOLUTION_H        852
    #define RESOLUTION_V        480
#else
    #define RESOLUTION_H        150
    #define RESOLUTION_V        100
#endif

sf::Color color_interpolation(sf::Color color1, sf::Color color2, double alpha) {
    if(color1==sf::Color::Transparent) return color2;
    if(color2==sf::Color::Transparent) return color1;

    return sf::Color(
        (1.f-alpha)*color1.r + alpha*color2.r,
        (1.f-alpha)*color1.g + alpha*color2.g,
        (1.f-alpha)*color1.b + alpha*color2.b
    );
}

class Ray : public Line {
public:
    Ray() {}

    Ray(Vector3 origin, Vector3 direction) : Line(origin, direction) {}

    bool intersect(Vector3 _point, double *t) {
        double d = distance(Line(point, direction), _point);
        if(d>EPSILON)
            return false;

        *t = direction*(_point-point);

        return true;
    }

};

class Transform3 : public Matrix {
private:
    Matrix inv;
    bool invReady;
public:

    Transform3() : Matrix(4, 4) {
        *this = Matrix::Identity(4);

        this->invReady = false;
    }

    Transform3 & operator=(const Matrix &rhs) {
        if(rhs.getRows()!=4) throw std::string("Can't assign matrix to transform");
        if(rhs.getCols()!=4) throw std::string("Can't assign matrix to transform");

        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                (*this)(i, j) = rhs(i, j);

        this->invReady = false;

        return *this;
    }

    void translate(Vector3 d) {
        Transform3 transform;

        transform(0, 3) = d.x;
        transform(1, 3) = d.y;
        transform(2, 3) = d.z;

        this->invReady = false;

        *this = (*this)*transform;
    }

    void rotate(Vector3 axis, double theta) {
        Transform3 transform;

        Matrix m(3, 3);
        m(0, 1) = -axis.z;
        m(0, 2) = axis.y;
        m(1, 0) = axis.z;
        m(1, 2) = -axis.x;
        m(2, 0) = -axis.y;
        m(2, 1) = axis.x;

        Matrix rot = m*std::sin(theta) + (Matrix::Identity(3) - axis*axis.transposition())*std::cos(theta) + (axis*axis.transposition());

        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                transform(i, j) = rot(i, j);

        this->invReady = false;

        *this = (*this)*transform;
    }

    Vector3 getTranslation() const {
        Vector3 result;
        result.x = (*this)(0, 3);
        result.y = (*this)(1, 3);
        result.z = (*this)(2, 3);
        return result;
    }

    Matrix getRotation() const {
        Matrix result(3, 3);
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                result(i, j) = (*this)(i, j);
        return result;
    }

    Matrix getInverse() {
        if(!this->invReady) {
            this->invReady = true;
            this->inv = this->inverse();
        }

        return this->inv;
    }

};

class CollisionData {
public:
    sf::Color color;
    Vector3 normal;
    Vector3 point;
    double reflectivity;

    CollisionData() {}

};

class Object {
public:
    Object() {}

    virtual bool intersect(Ray ray, CollisionData *data) {
        return false;
    }

};

class Camera {
public:
    Vector3 position;
    Vector3 directionX, directionY, directionZ;
    double distance;
    Plane screen;
    std::vector<Ray> rays;

    Camera() {}

    Camera(Vector3 _position, Vector3 _directionX, double _distance) {
        position = _position;
        distance = _distance;
        directionX = normalize(_directionX);

        directionX = normalize(directionX);
        directionY = normalize(Vector3(0, 0, 1)^directionX);
        directionZ = normalize(directionY^directionX);

        update_rays();
    }

    void move(sf::Vector2i mouse) {
        Vector3 p = position;
        Vector3 dx = directionX;
        Vector3 dy = directionY;
        Vector3 dz = directionZ;

        Vector3 translation(0, 0, 0);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))         translation +=directionX;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))         translation -=directionX;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))         translation -=directionY;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))         translation +=directionY;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))    translation -=directionZ;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))  translation +=directionZ;
        position +=LINEAR_VELOCITY*normalize(translation);

        if(mouse.x<0) {
            directionX = rotate(directionZ, directionX, ANGULAR_VELOCITY);
            directionY = rotate(directionZ, directionY, ANGULAR_VELOCITY);
        } else if(mouse.x>0) {
            directionX = rotate(directionZ, directionX, -ANGULAR_VELOCITY);
            directionY = rotate(directionZ, directionY, -ANGULAR_VELOCITY);
        }

        if(mouse.y>0) {
            directionX = rotate(directionY, directionX, ANGULAR_VELOCITY);
            directionZ = rotate(directionY, directionZ, ANGULAR_VELOCITY);
        } else if(mouse.y<0) {
            directionX = rotate(directionY, directionX, -ANGULAR_VELOCITY);
            directionZ = rotate(directionY, directionZ, -ANGULAR_VELOCITY);
        }

        if((directionY*Vector3(0, 0, 1))<-0.1) {
            directionY = rotate(directionX, directionY, ANGULAR_VELOCITY);
            directionZ = rotate(directionX, directionZ, ANGULAR_VELOCITY);
        } else if((directionY*Vector3(0, 0, 1))>0.1) {
            directionY = rotate(directionX, directionY, -ANGULAR_VELOCITY);
            directionZ = rotate(directionX, directionZ, -ANGULAR_VELOCITY);
        }

        if(position!=p || directionX!=dx || directionY!=dy || directionZ!=dz) {
            update_rays();
        }

    }

    void update_rays() {
        directionX = normalize(directionX);
        directionY = normalize(directionY);
        directionZ = normalize(directionZ);

        screen = Plane(position + distance*directionX, directionX);
        rays.resize(RESOLUTION_H*RESOLUTION_V);

        for(int i=0; i<RESOLUTION_V; i++) {
            for(int j=0; j<RESOLUTION_H; j++) {
                Vector3 dir = normalize(
                    distance*directionX +
                    (j-RESOLUTION_H/2.)/RESOLUTION_H*WINDOW_WIDTH*directionY +
                    (i-RESOLUTION_V/2.)/RESOLUTION_V*WINDOW_HEIGHT*directionZ
                );

                rays[i*RESOLUTION_H + j] = Ray(position, dir);
            }
        }
    }

};

class Sphere : public Object {
public:
    sf::Image *texture;
    sf::Color color;
    Transform3 transform;
    double radius;
    double reflectivity;

    Sphere() {}

    Sphere(Vector3 _center, double _radius, double _reflectivity) {
        transform.translate(_center);
        radius = _radius;
        reflectivity = _reflectivity;

        color = sf::Color(rand()%256, rand()%256, rand()%256);
        texture = nullptr;
    }

    Sphere(Vector3 _center, double _radius, double _reflectivity, sf::Image *_texture) {
        transform.translate(_center);
        radius = _radius;
        reflectivity = _reflectivity;

        color = sf::Color::Transparent;
        texture = _texture;
    }

    sf::Color getPixel(Vector3 point) {

        Matrix p(4, 1);
        p(0, 0) = point.x;
        p(1, 0) = point.y;
        p(2, 0) = point.z;
        p(3, 0) = 1;

        Matrix rel = transform.getInverse()*p;

        Vector3 relative(
            rel(0, 0),
            rel(1, 0),
            rel(2, 0)
        );

        if(texture!=nullptr) {
            Vector3 d = normalize(relative);

            double u = 0.5f + std::atan2(d.x, d.y)/(2.f*M_PI);
            double v = 0.5f - std::asin(d.z)/M_PI;

            return texture->getPixel(u*texture->getSize().x, v*texture->getSize().y);
        }

        return color;
    }

    bool intersect(Ray ray, CollisionData *data) {
        Vector3 center = transform.getTranslation();

        double delta = pow(ray.direction*(ray.point-center), 2) - (pow(length(ray.point-center), 2) - pow(radius, 2));
        double t = -(ray.direction*(ray.point-center));

        if(delta<0.f)
            return false;

        double t1 = t + sqrt(delta);
        double t2 = t - sqrt(delta);

        if(t1<EPSILON && t2<EPSILON)
            return false;

        double t_nearest_positive = ((t1*t2<0) ? std::max(t1, t2) : std::min(t1, t2));

        data->point = ray.point + t_nearest_positive*ray.direction;
        data->color = getPixel(data->point);
        data->normal = normalize(data->point - center);
        data->reflectivity = reflectivity;

        return true;
    }

};

class Triangle : public Object {
public:
    Vector3 v0, v1, v2;
    sf::Color color;
    double reflectivity;

    Triangle() {}

    Triangle(Vector3 _v0, Vector3 _v1, Vector3 _v2, double _reflectivity) {
        v0 = _v0;
        v1 = _v1;
        v2 = _v2;
        reflectivity = _reflectivity;
        color = sf::Color(rand()%256, rand()%256, rand()%256);
    }

    bool intersect(Ray ray, CollisionData *data) {
        //Möller–Trumbore intersection algorithm

        Vector3 edge1 = v1 - v0;
        Vector3 edge2 = v2 - v0;
        Vector3 h = ray.direction^edge2;

        double a = edge1*h;
        if (a>-EPSILON && a<EPSILON)
            return false;    // This ray is parallel to this triangle.

        double f = 1.0/a;
        Vector3 s = ray.point - v0;
        double u = f * (s*h);
        if(u<0.0 || u>1.0)
            return false;

        Vector3 q = s^edge1;
        double v = f*(ray.direction*q);
        if(v<0.0 || u+v>1.0)
            return false;

        double t = f*(edge2*q);
        if(t>EPSILON) {
            data->point = ray.point + t*ray.direction;
            data->normal = normalize(edge1^edge2);
            data->color = color;
            data->reflectivity = reflectivity;
            return true;
        }

        // This means that there is a line intersection but not a ray intersection.
        return false;
    }

};

class Ground : public Object, public Plane {
public:
    sf::Image *texture;
    double textWidth, textHeight;
    double reflectivity;

    Ground() {}

    Ground(Vector3 point, Vector3 normal, double _reflectivity) : Plane(point, normal) {
        reflectivity = _reflectivity;
        texture = nullptr;
    }

    Ground(Vector3 point, Vector3 normal, double _reflectivity, sf::Image *_texture, double _textWidth, double _textHeight) : Plane(point, normal) {
        reflectivity = _reflectivity;

        texture = _texture;
        textWidth = _textWidth;
        textHeight = _textHeight;
    }

    sf::Color getPixel(Vector3 point3D) {
        //only special case
        sf::Vector2f point2D(point3D.x, point3D.y);

        if(texture==nullptr) {
            bool gridX = round(fabs(fmod(point2D.x/50, 1))); if(point2D.x<0) gridX = !gridX;
            bool gridY = round(fabs(fmod(point2D.y/50, 1))); if(point2D.y<0) gridY = !gridY;

            return !gridX != !gridY ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50);
        }

        sf::Vector2f textCoords(
            std::fmod(point2D.x, textWidth),
            std::fmod(point2D.y, textHeight)
        );

        if(textCoords.x<0) textCoords.x +=textWidth;
        if(textCoords.y<0) textCoords.y +=textHeight;

        return texture->getPixel(
            (textCoords.x/textWidth)*texture->getSize().x,
            (textCoords.y/textHeight)*texture->getSize().y
        );
    }

    bool intersect(Ray ray, CollisionData *data) {
        if(Plane::intersect(ray, &data->point)) {
            if((ray.direction*(data->point-ray.point))<0)
                return false;

            data->normal = getNormal();
            data->color = getPixel(data->point);
            data->reflectivity = reflectivity;
            return true;
        }
        return false;
    }

};

class Scene {
public:
    std::vector<Object*> objects;
    Camera *camera;

    Scene() {}

    Scene(Camera *_camera) {
        camera = _camera;
    }

    sf::Color trace(Ray ray, int depth) {
        if(depth<=0)
            return sf::Color::Transparent;

        CollisionData tmp, data;
        double d, dist = 100000;
        bool exist = false;

        for(int i=0; i<objects.size(); i++) {
            if(objects[i]->intersect(ray, &tmp)) {
                if(length(tmp.point-ray.point)>EPSILON) {
                    d = length(tmp.point-ray.point);
                    if(d<dist) {
                        dist = d;
                        data = tmp;
                    }
                    exist = true;
                }
            }
        }

        if(exist) {
            //sf::Color color_reflected = trace(Ray(point, ray.direction - 2.f*dot(ray.direction, normal)*normal), depth-1);
            sf::Color color_reflected = (data.reflectivity>EPSILON) ?
                trace(Ray(data.point, ray.direction - 2.f*(ray.direction*data.normal)*data.normal), depth-1) :
                sf::Color::Transparent;

            //double brightness = dot(light_ray.direction, normal);

            //return color_hit;
            //return color_reflected;
            //return sf::Color(brightness, brightness, brightness);
            //return (shadow || brightness<0) ? sf::Color::Black : sf::Color::White;

            return color_interpolation(data.color, color_reflected, data.reflectivity);
        }

        return sf::Color::Transparent;
    }

    void add(Object *object_ptr) {
        objects.push_back(object_ptr);
    }

    void clear() {
        objects.clear();
    }

    void render(sf::RenderWindow &window) {
        const double pixel_w = (double)window.getSize().x/RESOLUTION_H;
        const double pixel_h = (double)window.getSize().y/RESOLUTION_V;

        sf::RectangleShape pixel(sf::Vector2f(pixel_w, pixel_h));

        for(int i=0; i<RESOLUTION_V; i++) {
            for(int j=0; j<RESOLUTION_H; j++) {
                sf::Color color = trace(camera->rays[i*RESOLUTION_H + j], REFLECTION_DEPTH);

                if(color!=sf::Color::Transparent && color!=sf::Color::Black) {
                    pixel.setFillColor(color);
                    pixel.setPosition(sf::Vector2f(j*pixel_w, i*pixel_h));

                    window.draw(pixel);
                }
            }
        }

    }

};

class TextureMenager {
public:
    std::vector<sf::Image> textures;

    TextureMenager() {}

    void load(std::string texture_str) {
        sf::Image texture;
        texture.loadFromFile(texture_str);
        textures.push_back(texture);
    }

    sf::Image* getTextureReference(int index) {
        return &textures[index];
    }

};

int main() {

    srand(time(NULL));

    sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ray Tracing");
	window.setMouseCursorVisible(false);

	Camera camera(Vector3(-60, 20, 20), Vector3(1, 0, 0), 1000);
	Scene scene(&camera);

	TextureMenager menager;
	menager.load("textures/road1.jpg");
	menager.load("textures/earth2.jpg");
	menager.load("textures/notexture.jpg");

    Sphere skybox(Vector3(0, 0, 0), 10000, 0, menager.getTextureReference(0));
	Ground ground(Vector3(0, 0, 0), Vector3(0, 0, 1), 0, menager.getTextureReference(2), 50, 50);
	Triangle mirror(Vector3(50, 100, 0), Vector3(50, 10, 0), Vector3(100, 10, 100), 0.3);

	Sphere balls[9];
	for(int i=0; i<9; i++)
        balls[i] = Sphere(Vector3((i/3)*20, (i%3)*20, 20), 7, 0.3);

    balls[0].texture = menager.getTextureReference(1);
    balls[0].reflectivity = 0.15;

    double angle = 0;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type==sf::Event::Closed)
                window.close();
            if(event.type==sf::Event::KeyPressed)
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                    window.close();
        }

        sf::Vector2i center = sf::Vector2i(window.getSize().x, window.getSize().y)/2;
        sf::Vector2i mouse = sf::Mouse::getPosition(window) - center;
        sf::Mouse::setPosition(center, window);
        camera.move(mouse);

		window.clear();
		scene.clear();

		balls[0].transform = Transform3();

        //simple earth simulation
		balls[0].transform.translate(Vector3(-10, -10, 20) + rotate(Vector3(0, 0, 1), Vector3(10, 10, 0), angle));
		balls[0].transform.rotate(Vector3(0, 1, 1), M_PI/3);
        balls[0].transform.rotate(Vector3(0, 0, 1), 5*angle);
		angle +=0.1;

		//scene.add(&balls[0]);
        for(int i=0; i<9; i++)
            scene.add(&balls[i]);
		//scene.add(&skybox);
		//scene.add(&ground);
		scene.add(&mirror);

		scene.render(window);

		double renderTime = clock.restart().asSeconds();

		std::string windowTitle = "Ray Tracing";
		windowTitle +=" | Resolution: " + std::to_string(RESOLUTION_H) + "x" + std::to_string(RESOLUTION_V);
        windowTitle +=" | Render Time: " + std::to_string(renderTime) + "s";
		windowTitle +=" | FPS: " + std::to_string(1/renderTime);
		window.setTitle(windowTitle);

        window.display();
    }

}
