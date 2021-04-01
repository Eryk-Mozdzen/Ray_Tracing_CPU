#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Tools.h"
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

    Ray(Vector3d origin, Vector3d direction) : Line(origin, direction) {}

    bool intersect(Vector3d _point, double *t) {
        double d = Line::distance(Line(point, direction), _point);
        if(d>EPSILON)
            return false;

        *t = dot(direction, _point-point);

        return true;
    }

};

class CollisionData {
public:
    sf::Color color;
    Vector3d normal;
    Vector3d point;
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
    Vector3d position;
    Vector3d directionX, directionY, directionZ;
    double distance;
    Plane screen;
    std::vector<Ray> rays;

    Camera() {}

    Camera(Vector3d _position, Vector3d _directionX, double _distance) {
        position = _position;
        distance = _distance;
        directionX = normalize(_directionX);

        directionX = normalize(directionX);
        directionY = normalize(cross(Vector3d(0, 0, 1), directionX));
        directionZ = normalize(cross(directionY, directionX));

        update_rays();
    }

    void move(sf::Vector2i mouse) {
        Vector3d p = position;
        Vector3d dx = directionX;
        Vector3d dy = directionY;
        Vector3d dz = directionZ;

        Vector3d translation(0, 0, 0);
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

        if(dot(directionY, Vector3d(0, 0, 1))<-0.1) {
            directionY = rotate(directionX, directionY, ANGULAR_VELOCITY);
            directionZ = rotate(directionX, directionZ, ANGULAR_VELOCITY);
        } else if(dot(directionY, Vector3d(0, 0, 1))>0.1) {
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
                /*Vector3d dir = normalize(
                    distance*directionX +
                    (j-RESOLUTION_H/2.f)*directionY +
                    (i-RESOLUTION_V/2.f)*directionZ
                );*/

                Vector3d dir = normalize(
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
    Vector3d center;
    double radius;
    double reflectivity;

    Sphere() {}

    Sphere(Vector3d _center, double _radius, double _reflectivity) {
        center = _center;
        radius = _radius;
        reflectivity = _reflectivity;
        color = sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255));
        texture = nullptr;
    }

    Sphere(Vector3d _center, double _radius, double _reflectivity, sf::Image *_texture) {
        center = _center;
        radius = _radius;
        reflectivity = _reflectivity;
        color = sf::Color::Transparent;
        texture = _texture;
    }

    sf::Color getPixel(Vector3d point) {
        if(texture!=nullptr) {
            Vector3d d = normalize(point-center);

            double u = 0.5f + atan2(d.x, d.y)/(2.f*PI);
            double v = 0.5f - asin(d.z)/PI;

            return texture->getPixel(u*texture->getSize().x, v*texture->getSize().y);
        }

        return color;
    }

    bool intersect(Ray ray, CollisionData *data) {

        double delta = pow(dot(ray.direction, ray.point-center), 2) - (pow(length(ray.point-center), 2) - pow(radius, 2));
        double t = -dot(ray.direction, ray.point-center);

        if(delta<0.f)
            return false;

        double t1 = t + sqrt(delta);
        double t2 = t - sqrt(delta);

        if(t1<EPSILON && t2<EPSILON)
            return false;

        double t_nearest_positive = (sgn(t1)!=sgn(t2)) ? ((t1>0.f) ? t1 : t2) : std::min(t1, t2);

        data->point = ray.point + t_nearest_positive*ray.direction;
        data->color = getPixel(data->point);
        data->normal = normalize(data->point - center);
        data->reflectivity = reflectivity;

        return true;
    }

};

class Triangle : public Object {
public:
    Vector3d v0, v1, v2;
    sf::Color color;
    double reflectivity;

    Triangle() {}

    Triangle(Vector3d _v0, Vector3d _v1, Vector3d _v2, double _reflectivity) {
        v0 = _v0;
        v1 = _v1;
        v2 = _v2;
        reflectivity = _reflectivity;
        color = sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255));
    }

    bool intersect(Ray ray, CollisionData *data) {
        //Möller–Trumbore intersection algorithm

        Vector3d edge1 = v1 - v0;
        Vector3d edge2 = v2 - v0;
        Vector3d h = cross(ray.direction, edge2);

        double a = dot(edge1, h);
        if (a>-EPSILON && a<EPSILON)
            return false;    // This ray is parallel to this triangle.

        double f = 1.0/a;
        Vector3d s = ray.point - v0;
        double u = f * dot(s, h);
        if(u<0.0 || u>1.0)
            return false;

        Vector3d q = cross(s, edge1);
        double v = f*dot(ray.direction, q);
        if(v<0.0 || u+v>1.0)
            return false;

        double t = f*dot(edge2, q);
        if(t>EPSILON) {
            data->point = ray.point + t*ray.direction;
            data->normal = normalize(cross(edge1, edge2));
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
    double reflectivity;

    Ground() {}

    Ground(Vector3d point, Vector3d normal, double _reflectivity) : Plane(point, normal) {
        reflectivity = _reflectivity;
    }

    sf::Color getPixel(Vector3d point3D) {
        sf::Vector2f point2D(point3D.x, point3D.y);

        bool gridX = round(fabs(fmod(point2D.x/50, 1))); if(point2D.x<0) gridX = !gridX;
        bool gridY = round(fabs(fmod(point2D.y/50, 1))); if(point2D.y<0) gridY = !gridY;

        return !gridX != !gridY ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50);
    }

    bool intersect(Ray ray, CollisionData *data) {
        if(Plane::intersect(ray, &data->point)) {
            if(dot(ray.direction, data->point-ray.point)<0)
                return false;

            data->normal = getNormal();
            data->color = getPixel(data->point);
            data->reflectivity = reflectivity;
            return true;
        }
        return false;
    }

};

class Light {
public:
    Vector3d position;
    sf::Color color;
    double brightness;

    Light() {}

    Light(Vector3d _position, sf::Color _color, double _brightness) {
        position = _position;
        color = _color;
        brightness = _brightness;
    }

};

class Scene {
public:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
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
                trace(Ray(data.point, ray.direction - 2.f*dot(ray.direction, data.normal)*data.normal), depth-1) :
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

    void render(sf::RenderWindow *window) {
        const double pixel_w = (double)window->getSize().x/RESOLUTION_H;
        const double pixel_h = (double)window->getSize().y/RESOLUTION_V;

        sf::RectangleShape pixel(sf::Vector2f(pixel_w, pixel_h));

        for(int i=0; i<RESOLUTION_V; i++) {
            for(int j=0; j<RESOLUTION_H; j++) {
                sf::Color color = trace(camera->rays[i*RESOLUTION_H + j], REFLECTION_DEPTH);

                if(color!=sf::Color::Transparent && color!=sf::Color::Black) {
                    pixel.setFillColor(color);
                    pixel.setPosition(sf::Vector2f(j*pixel_w, i*pixel_h));

                    window->draw(pixel);
                }
            }
        }

    }

};

class DataDisplay {
public:
    sf::Clock clock;
    sf::Font font;
    sf::Text text;
    double alpha;
    double dT_filter;
    std::string str;

    DataDisplay() {}

    DataDisplay(double _alpha) {
        alpha = _alpha;
        dT_filter = 0;
        str = "";

        if(!font.loadFromFile("arial.ttf")) {}
        text.setFont(font);
        text.setCharacterSize(18);
    }

    void update(sf::RenderWindow *win, sf::Vector2f pos) {
        double dT = clock.restart().asSeconds();
        dT_filter = alpha*dT + (1.f-alpha)*dT_filter;
        text.setString("FPS: " + std::to_string(1.f/dT_filter) + "\nTime: " + std::to_string(dT_filter) + "\n" + str);
        text.setPosition(pos);
        win->draw(text);
        str = "";
    }

    void add(std::string name, Vector3d v) {
        str +=(name + std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z) + "\n");
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

    DataDisplay data_display(1);
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ray Tracing");
	window.setMouseCursorVisible(false);

	Camera camera(Vector3d(-60, 20, 20), Vector3d(1, 0, 0), 1000);
	Scene scene(&camera);

	TextureMenager menager;
	menager.load("textures/road1.jpg");
	menager.load("textures/earth2.jpg");

    Sphere skybox(Vector3d(0, 0, 0), 10000, 0, menager.getTextureReference(0));
	Ground ground(Vector3d(0, 0, 0), Vector3d(0, 0, 1), 0);
	Triangle mirror(Vector3d(50, 100, 0), Vector3d(50, 10, 0), Vector3d(100, 10, 100), 1);

	Sphere balls[9];
	for(int i=0; i<9; i++)
        balls[i] = Sphere(Vector3d((i/3)*20, (i%3)*20, 20), 7, 0.3);

    balls[0].texture = menager.getTextureReference(1);
    balls[0].reflectivity = 0.15;

    //scene.lights.push_back(new Light(Vector3d(0, 0, 50), sf::Color(255, 255, 255), 0.1));

    double angle = 0;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type==sf::Event::Closed)
                window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                window.close();
        }

        sf::Vector2i center = sf::Vector2i(window.getSize().x, window.getSize().y)/2;
        sf::Vector2i mouse = sf::Mouse::getPosition(window) - center;
        sf::Mouse::setPosition(center, window);
        camera.move(mouse);

        //skybox.center = camera.position;

		window.clear();
		scene.clear();

		balls[0].center = Vector3d(20*sin(angle), -20 + 20*cos(angle), 20);
		angle +=0.1;

        for(int i=0; i<9; i++)
            scene.add(&balls[i]);
		scene.add(&skybox);
		scene.add(&ground);
		scene.add(&mirror);

		scene.render(&window);

        data_display.add("Position: ", camera.position);
        data_display.add("Direction X: ", camera.directionX);
        data_display.add("Direction Y: ", camera.directionY);
        data_display.add("Direction Z: ", camera.directionZ);
        data_display.update(&window, sf::Vector2f(0, 0));
        window.display();
    }

    return 0;
}
