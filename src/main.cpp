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
public:
    double radius;

    Sphere() {
        this->radius = 1;
    }

    Sphere(const Vector3 &center, const double &radius) {
        this->radius = radius;
        this->transform.translate(center);
        this->material = Material(rand()%256, rand()%256, rand()%256);
    }

    Sphere(const Vector3 &center, const double &radius, sf::Image *texture) {
        this->radius = radius;
        this->transform.translate(center);
        this->material = Material(texture);
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

        double delta = pow(ray.direction*(ray.point-center), 2) - (pow(length(ray.point-center), 2) - pow(radius, 2));
        double t = -(ray.direction*(ray.point-center));

        if(delta<0)
            return false;

        double t1 = t + sqrt(delta);
        double t2 = t - sqrt(delta);

        if(t1<EPSILON && t2<EPSILON)
            return false;

        double t_nearest_positive = ((t1*t2<0) ? std::max(t1, t2) : std::min(t1, t2));

        data.point = ray.point + t_nearest_positive*ray.direction;
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

    Ground() : Plane() {}

    Ground(Vector3 point, Vector3 normal, double reflectivity) : Plane(point, normal) {
        this->material.setParameters(reflectivity, 0, 0, 0, 0.1);
    }

    Ground(Vector3 point, Vector3 normal, sf::Image *texture, double textWidth, double textHeight) : Plane(point, normal) {
        this->material = Material(texture, textWidth, textHeight);
    }

    sf::Color getPixel(Vector3 point3D) {
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

    /*std::cout << std::setprecision(4) << std::fixed;

    Matrix m1(4, 4);
    for(int i=0; i<m1.getRows(); i++)
        for(int j=0; j<m1.getCols(); j++)
            m1(i, j) = rand()%10;

    std::cout << m1 << std::endl;

    std::cout << (double)Matrix::Det(m1) << std::endl;

    return 0;*/

    try {

    srand(time(NULL));

    sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ray Tracing");

	window.setMouseCursorVisible(false);
	sf::Vector2i center = sf::Vector2i(window.getSize().x, window.getSize().y)/2;
	sf::Mouse::setPosition(center, window);

	View view(Vector3(-60, 20, 20), 1);
	Scene scene;

	TextureMenager menager;
	menager.load("textures/road1.jpg");
	menager.load("textures/earth2.jpg");
	menager.load("textures/notexture.jpg");
	menager.load("textures/wood2.jpg");

    Sphere skybox(Vector3(0, 0, 0), 10000, menager.getTextureReference(0));
	Ground ground(Vector3(0, 0, 0), Vector3(0, 0, 1), menager.getTextureReference(2), 5000, 5000);
	Triangle mirror(Vector3(50, 100, 0), Vector3(50, 10, 0), Vector3(100, 10, 100));

	Sphere balls[9];
	for(int i=0; i<9; i++)
        balls[i] = Sphere(Vector3((i/3)*20, (i%3)*20, 20), 7);

    //balls[0].material = Material(menager.getTextureReference(1));

    for(int i=0; i<1; i++)
        scene.addObject(&balls[i]);
    //scene.addObject(&skybox);
    scene.addObject(&ground);
    //scene.addObject(&mirror);

    //scene.addLightSource(new LightSource(Vector3(-60, 20, 20)));
    scene.addLightSource(new LightSource(Vector3(0, 20, 60)));
    scene.addLightSource(new LightSource(Vector3(0, 40, 60)));

    double angle = 0;
    double distance = 1;

    int resolutionH = 15;
    int resolutionV = 10;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type==sf::Event::Closed)
                window.close();
            if(event.type==sf::Event::KeyPressed)
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                    window.close();
            /*if(event.type==sf::Event::MouseWheelScrolled) {
                if(event.mouseWheelScroll.delta>0) distance +=((distance>=1)? 1 : 0.1);
                else if(distance>0.2)              distance -=((distance>1)? 1 : 0.1);
            }*/
            if(event.type==sf::Event::MouseWheelScrolled) {
                if(event.mouseWheelScroll.delta>0) {resolutionH +=15; resolutionV +=10;}
                else                               {resolutionH -=15; resolutionV -=10;}
            }
        }

        sf::Vector2i deltaMouse = sf::Mouse::getPosition(window) - center;
        sf::Mouse::setPosition(center, window);

        viewMove(view, deltaMouse);
        view.setDistanceFromProjectionPlane(distance);

/*        //simple earth simulation
		Transform3 tr_original = balls[0].transform;            //safe original transformation
		balls[0].transform.translate(Vector3(-10, -10, 0));     //move to orbit center
		balls[0].transform.rotate(Vector3(0, 0, 1), angle);     //rotate by some angle in vertical axis
		balls[0].transform.translate(Vector3(10, 0, 0));        //move to orbit position
		balls[0].transform.rotate(Vector3(0, 0, 1), -angle);    //return to start angle
		balls[0].transform.rotate(Vector3(0, 1, 1), M_PI/3);    //tilt self-rotate axis
        balls[0].transform.rotate(Vector3(0, 0, 1), 5*angle);   //rotate in self axis
		angle +=0.1;                                            //increment angle
*/
		clock.restart();

		sf::Image frame = scene.render(view, resolutionH, resolutionV);

		double renderTime = clock.restart().asSeconds();

//		balls[0].transform = tr_original;                       //return to original transformation

		std::stringstream windowTitle;
		windowTitle << std::setprecision(5) << std::fixed;
		windowTitle << "Ray Tracing";
		windowTitle << " | Resolution: " << resolutionH << "x" << resolutionV;
        windowTitle << " | Render Time: " << renderTime << "s";
		windowTitle << " | FPS: " << 1/renderTime;
		windowTitle << " | Zoom: " << view.getDistanceFromProjectionPlane();
		window.setTitle(windowTitle.str());

		window.clear(sf::Color(32, 32, 32));

		frame.flipVertically();
		frame.flipHorizontally();
        sf::Texture tex;
        tex.loadFromImage(frame);
        sf::Sprite sprite;
        sprite.setTexture(tex);
        sprite.scale((double)WINDOW_WIDTH/resolutionH, (double)WINDOW_HEIGHT/resolutionV);
		window.draw(sprite);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            frame.saveToFile("saved_frame.jpg");
            std::cout << "Saved frame" << std::endl;
		}

        window.display();
    }

    } catch(std::string e) {
        std::cout << e << std::endl;
    }

}
