#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../include/Vector.h"
#include "../include/Matrix.h"
#include "../include/geometry.h"

#include "../include/RayTracingConfig.h"
#include "../include/RayTracingUtilities.h"
#include "../include/RayTracing.h"

#define WINDOW_WIDTH        1280
#define WINDOW_HEIGHT       720

#define LINEAR_VELOCITY     1.5
#define ANGULAR_VELOCITY    0.1

void cameraMove(Camera &camera, const sf::Vector2i &mouse) {
    Transform3 tr = camera.transform;

    Vector3 translation(0, 0, 0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))         translation +=Vector3::UnitX();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))         translation -=Vector3::UnitX();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))         translation +=Vector3::UnitY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))         translation -=Vector3::UnitY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))    translation +=Vector3::UnitZ();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))  translation -=Vector3::UnitZ();

    camera.transform.translate(LINEAR_VELOCITY*normalize(translation));

    if(std::abs(mouse.x)>5)
        camera.transform.rotate(Vector3::UnitZ(), ANGULAR_VELOCITY*((mouse.x<0)? 1 : -1));

    if(std::abs(mouse.y)>5)
        camera.transform.rotate(Vector3::UnitY(), ANGULAR_VELOCITY*((mouse.y>0)? 1 : -1));

    double s = camera.getDirectionY()*Vector3::UnitZ();
    if(std::abs(s)>0.1)
        camera.transform.rotate(Vector3::UnitX(), ANGULAR_VELOCITY*((s<0)? 1 : -1));

    //if(camera.transform!=tr)
        camera.update_rays();
}

/*-----------  User custom drawable objects  ---------------*/

class Sphere : public Object {
public:
    sf::Image *texture;
    sf::Color color;
    double reflectivity;
    Transform3 transform;
    double radius;

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

    bool intersect(const Ray &ray, CollisionData *data) {
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

    bool intersect(const Ray &ray, CollisionData *data) {
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

    bool intersect(const Ray &ray, CollisionData *data) {
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

int main() {

    try {

    srand(time(NULL));

    sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ray Tracing");
	//window.setFramerateLimit(8);
	sf::View view = window.getView();
    view.setRotation(180);
    window.setView(view);

	window.setMouseCursorVisible(false);
	sf::Vector2i center = sf::Vector2i(window.getSize().x, window.getSize().y)/2;
	sf::Mouse::setPosition(center, window);

	Camera camera(Vector3(-60, 20, 20), 1);
	Scene scene;

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
            if(event.type==sf::Event::MouseWheelScrolled) {
                if(event.mouseWheelScroll.delta>0)  camera.distance +=((camera.distance>=1)? 1 : 0.1);
                else if(camera.distance>0.2)        camera.distance -=((camera.distance>1)? 1 : 0.1);
            }
        }

        sf::Vector2i deltaMouse = sf::Mouse::getPosition(window) - center;
        sf::Mouse::setPosition(center, window);
        cameraMove(camera, deltaMouse);

		window.clear();
		scene.clear();

		Transform3 tr_original = balls[0].transform;            //safe original transformation

        //simple earth simulation
		balls[0].transform.translate(Vector3(-10, -10, 0));     //move to orbit center
		balls[0].transform.rotate(Vector3(0, 0, 1), angle);     //rotate by some angle in vertical axis
		balls[0].transform.translate(Vector3(10, 0, 0));        //move to orbit position
		balls[0].transform.rotate(Vector3(0, 0, 1), -angle);    //return to start angle
		balls[0].transform.rotate(Vector3(0, 1, 1), M_PI/3);    //tilt self-rotate axis
        balls[0].transform.rotate(Vector3(0, 0, 1), 5*angle);   //rotate in self axis
		angle +=0.1;                                            //increment angle

        for(int i=0; i<9; i++)
            scene.add(&balls[i]);
		//scene.add(&skybox);
		scene.add(&ground);
		//scene.add(&mirror);

		scene.render(camera, window);

		balls[0].transform = tr_original;                       //return to original transformation

		double renderTime = clock.restart().asSeconds();

		std::stringstream windowTitle;
		windowTitle << std::setprecision(5) << std::fixed;
		windowTitle << "Ray Tracing";
		windowTitle << " | Resolution: " << RESOLUTION_H << "x" << RESOLUTION_V;
        windowTitle << " | Render Time: " << renderTime << "s";
		windowTitle << " | FPS: " << 1/renderTime;
		//windowTitle << " | Position: " << camera.transform.getTranslation();
		//windowTitle << " | dirX: " << camera.getDirectionX();
		windowTitle << " | Zoom: " << camera.distance;
		window.setTitle(windowTitle.str());

        window.display();
    }

    } catch(std::string e) {
        std::cout << e << std::endl;
    }

}
