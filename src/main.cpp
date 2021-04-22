#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../include/RayTracing.h"

#define WINDOW_WIDTH        1280
#define WINDOW_HEIGHT       720

#define LINEAR_VELOCITY     1.5
#define ANGULAR_VELOCITY    0.1

void viewMove(View &view, const sf::Vector2i &mouse) {
    Vector3 translation(0, 0, 0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))         translation +=Vector3::UnitX();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))         translation -=Vector3::UnitX();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))         translation +=Vector3::UnitY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))         translation -=Vector3::UnitY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))    translation +=Vector3::UnitZ();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))  translation -=Vector3::UnitZ();

    view.translate(LINEAR_VELOCITY*normalize(translation));

    if(std::abs(mouse.x)>5)
        view.rotate(Vector3::UnitZ(), ANGULAR_VELOCITY*((mouse.x<0)? 1 : -1));

    if(std::abs(mouse.y)>5)
        view.rotate(Vector3::UnitY(), ANGULAR_VELOCITY*((mouse.y>0)? 1 : -1));

    double s = view.getDirectionY()*Vector3::UnitZ();
    if(std::abs(s)>0.1)
        view.rotate(Vector3::UnitX(), ANGULAR_VELOCITY*((s<0)? 1 : -1));
}

/*-----------  User custom drawable objects  ---------------*/

class Sphere : public Object {
private:
    double radius;
public:

    Sphere() {
        this->radius = 1;
    }

    Sphere(const Vector3 &center, const double &radius) {
        this->radius = radius;
        this->transform.translate(center);
        this->material = Material(rand()%256, rand()%256, rand()%256);
    }

    sf::Color getPixel(const Vector3 &point) {
        Vector3 relative = this->transform.getRelativeToTransform(point);

        Vector3 d = normalize(relative);

        double u = 0.5f + std::atan2(d.x, d.y)/(2.f*M_PI);
        double v = 0.5f - std::asin(d.z)/M_PI;

        return this->material.getColorAt(u, v);
    }

    bool intersect(const Ray &ray, CollisionData &data) {
        Vector3 center = transform.getTranslation();

        double delta = std::pow(ray.direction*(ray.point-center), 2) - (std::pow(length(ray.point-center), 2) - std::pow(radius, 2));
        double t = -(ray.direction*(ray.point-center));

        if(delta<EPSILON)
            return false;

        double t1 = t + std::sqrt(delta);
        double t2 = t - std::sqrt(delta);

        //don't work, precision error
        //double tNearestPositive = ((t1*t2<0) ? std::max(t1, t2) : std::min(t1, t2));

        double tNearestPositive;
             if(t1>EPSILON && t2<EPSILON)   tNearestPositive = t1;
        else if(t1<EPSILON && t2>EPSILON)   tNearestPositive = t2;
        else if(t1>EPSILON && t2>EPSILON)   tNearestPositive = std::min(t1, t2);
        else                                return false;

        data.point = ray.point + tNearestPositive*normalize(ray.direction);
        data.normal = normalize(data.point - center);
        data.color = this->getPixel(data.point);
        data.material = this->material;
        data.distance = length(data.point - ray.point);
        data.exist = true;

        return true;
    }

};

class Triangle : public Object {
public:
    Vector3 v0, v1, v2;

    Triangle() {}

    Triangle(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) {
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
        this->material = Material(rand()%256, rand()%256, rand()%256);
    }

    bool intersect(const Ray &ray, CollisionData &data) {
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
            data.point = ray.point + t*ray.direction;
            data.normal = normalize(edge1^edge2);
            data.color = this->material.getColorAt(0, 0);
            data.material = this->material;
            data.distance = length(data.point - ray.point);
            data.exist = true;
            return true;
        }

        // This means that there is a line intersection but not a ray intersection.
        return false;
    }

};

class Ground : public Object, public Plane {
public:

    Ground(const Vector3 &point, const Vector3 &normal) : Plane(point, normal) {
        this->material.setAmbient(0);
        this->material.setDiffuse(1);
        this->material.setSpecular(0);
        this->material.setShininess(1);
        this->material.setReflection(0);
    }

    Ground(const Vector3 &point, const Vector3 &normal, sf::Image *texture, const double &textWidth, const double &textHeight) : Ground(point, normal) {
        this->material = Material(texture, textWidth, textHeight);
        this->material.setAmbient(0);
        this->material.setDiffuse(1);
        this->material.setSpecular(0);
        this->material.setShininess(1);
        this->material.setReflection(0.5);
    }

    sf::Color getPixel(const Vector3 &point3D) {
        //only special case
        sf::Vector2f point2D(point3D.x, point3D.y);

        return this->material.getColorAt(point3D.x, point3D.y);
    }

    bool intersect(const Ray &ray, CollisionData &data) {
        if(Plane::intersect(ray, &data.point)) {
            if((ray.direction*(data.point-ray.point))<0)
                return false;

            data.normal = this->getNormal();
            data.color = this->getPixel(data.point);
            data.material = this->material;
            data.distance = length(data.point - ray.point);
            data.exist = true;
            return true;
        }
        return false;
    }

};

int main() {

    srand(time(NULL));

    sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ray Tracing");

	window.setMouseCursorVisible(false);
	sf::Vector2i center = sf::Vector2i(window.getSize().x, window.getSize().y)/2;
	sf::Mouse::setPosition(center, window);

	View view(Vector3(-60, 20, 20), 1);
	RenderScene scene;

	scene.setReflectionDepth(3);

	TextureMenager menager;
	menager.load("textures/road1.jpg");
	menager.load("textures/earth2.jpg");
	menager.load("textures/notexture.jpg");

    //earth setup
	Sphere earth(Vector3(30, 0, 20), 7);
	earth.material = Material(menager.getTextureReference(1));
	earth.material.setAmbient(0);
	earth.material.setDiffuse(1);
	earth.material.setSpecular(0);
	earth.material.setReflection(0);

    //scene.addObject(&earth);
    scene.addObject(new Sphere(Vector3(0, 0, 20), 7));
    scene.addObject(new Sphere(Vector3(0, 20, 20), 7));
    scene.addObject(new Ground(Vector3(0, 0, 0), Vector3::UnitZ(), menager.getTextureReference(2), 5000, 5000));

    scene.addLightSource(new LightSource(Vector3(0, 0, 40)));
    scene.addLightSource(new LightSource(Vector3(-30, 0, 40)));

    double angle = 0;
    double scroll = 10;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type==sf::Event::Closed)
                window.close();
            if(event.type==sf::Event::KeyPressed)
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                    window.close();
            if(event.type==sf::Event::MouseWheelScrolled) {
                if(event.mouseWheelScroll.delta>0)  scroll++;
                else                                scroll--;
            }
        }

        sf::Vector2i deltaMouse = sf::Mouse::getPosition(window) - center;
        sf::Mouse::setPosition(center, window);

        scene.setRenderResolution(150, 100);
        view.setDistanceFromProjectionPlane(scroll/10);

        viewMove(view, deltaMouse);

        //simple earth orbit simulation
		Transform3 tr_original = earth.transform;            //safe original transformation
		earth.transform.translate(Vector3(-30, 0, 0));       //move to orbit center
		earth.transform.rotate(Vector3(0, 0, 1), angle);     //rotate by some angle in vertical axis
		earth.transform.translate(Vector3(30, 0, 0));        //move to orbit position
		earth.transform.rotate(Vector3(0, 0, 1), -angle);    //return to start angle
		earth.transform.rotate(Vector3(0, 1, 1), M_PI/6);    //tilt self-rotate axis
        earth.transform.rotate(Vector3(0, 0, 1), 5*angle);   //rotate in self axis

		clock.restart();

		sf::Image frameBuffer = scene.render(view);

		double renderTime = clock.restart().asSeconds();

		earth.transform = tr_original;                       //return to original transformation
		angle +=0.5*renderTime;                              //increment angle

		std::stringstream windowTitle;
		windowTitle << std::setprecision(5) << std::fixed;
		windowTitle << "Ray Tracing";
		windowTitle << " | Resolution: " << scene.getRenderResolutionWidth() << "x" << scene.getRenderResolutionHeight();
        windowTitle << " | Render Time: " << renderTime << "s";
		windowTitle << " | FPS: " << 1/renderTime;
		windowTitle << " | Zoom: " << view.getDistanceFromProjectionPlane();
		window.setTitle(windowTitle.str());

		window.clear(sf::Color::Green);

		frameBuffer.flipVertically();
		frameBuffer.flipHorizontally();
        sf::Texture tex;
        tex.loadFromImage(frameBuffer);
        sf::Sprite sprite;
        sprite.setTexture(tex);
        sprite.scale((double)WINDOW_WIDTH/scene.getRenderResolutionWidth(), (double)WINDOW_HEIGHT/scene.getRenderResolutionHeight());
		window.draw(sprite);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            frameBuffer.saveToFile("saved_frame.jpg");
            std::cout << "Saved frame" << std::endl;
		}

        window.display();
    }

}
