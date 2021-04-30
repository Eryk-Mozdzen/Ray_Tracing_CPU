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

#include "RayTracing.h"

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
    Material material;
    Transform3 transform;
public:

    Sphere() {
        this->radius = 1;
    }

    Sphere(const Vector3 &center, const double &radius) {
        this->radius = radius;
        this->transform.translate(center);
        this->material = Material(rand()%256, rand()%256, rand()%256);
    }

    Sphere(const Vector3 &center, const double &radius, const Material &material) : Sphere(center, radius) {
        this->material = material;
    }

    sf::Color getPixel(const Vector3 &point) const {
        if(!this->material.isTexture())
            return this->material.getColor();

        Vector3 relative = this->transform.getRelativeToTransform(point);

        Vector3 d = normalize(relative);

        double u = 0.5f + std::atan2(d.x, d.y)/(2.f*M_PI);
        double v = 0.5f - std::asin(d.z)/M_PI;

        return this->material.getColorAt(u, v);
    }

    const Transform3 & getTransform() const {
        return this->transform;
    }

    const Material & getMaterial() const {
        return this->material;
    }

    void setTransform(const Transform3 &transform) {
        this->transform = transform;
    }

    void setMaterial(const Material &material) {
        this->material = material;
    }

    CollisionData intersect(const Ray &ray) const {
        CollisionData data;
        const Vector3 center = transform.getTranslation();

        const double a = 1;
        const double b = 2*(ray.direction*(ray.origin - center));
        const double c = length(ray.origin - center)*length(ray.origin - center) - this->radius*this->radius;

        const std::pair<double, double> tSolution = solveQuadraticEquation(a, b, c);

        double tNearestPositive;
             if(tSolution.first>EPSILON && tSolution.second<EPSILON)   tNearestPositive = tSolution.first;
        else if(tSolution.first<EPSILON && tSolution.second>EPSILON)   tNearestPositive = tSolution.second;
        else if(tSolution.first>EPSILON && tSolution.second>EPSILON)   tNearestPositive = std::min(tSolution.first, tSolution.second);
        else return data;

        data.point = ray.origin + tNearestPositive*normalize(ray.direction);
        data.normal = normalize(data.point - center);
        data.color = this->getPixel(data.point);
        data.material = this->getMaterial();
        data.distance = length(data.point - ray.origin);
        data.exist = true;

        return data;
    }

    CollisionData distance(const Vector3 &point) const {
        CollisionData data;
        const double R = this->radius;
        const Vector3 center = this->transform.getTranslation();

        data.distance = length(point - this->transform.getTranslation()) - this->radius;
        data.point = point;
        data.normal = normalize(data.point - center);
        data.color = this->getPixel(normalize(point-center)*R + center);
        data.material = this->material;
        data.exist = (data.distance<EPSILON);
        
        return data;
    }

};

class Triangle : public Object {
private:
    Vector3 v0, v1, v2;
    Material material;
public:

    Triangle() {}

    Triangle(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) {
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
        this->material = Material(rand()%256, rand()%256, rand()%256);
    }

    bool intersect(const Ray &ray, CollisionData &data) const {
        //M�ller�Trumbore intersection algorithm

        Vector3 edge1 = v1 - v0;
        Vector3 edge2 = v2 - v0;
        Vector3 h = ray.direction^edge2;

        double a = edge1*h;
        if (a>-EPSILON && a<EPSILON)
            return false;    // This ray is parallel to this triangle.

        double f = 1.0/a;
        Vector3 s = ray.origin - v0;
        double u = f * (s*h);
        if(u<0.0 || u>1.0)
            return false;

        Vector3 q = s^edge1;
        double v = f*(ray.direction*q);
        if(v<0.0 || u+v>1.0)
            return false;

        double t = f*(edge2*q);
        if(t>EPSILON) {
            data.point = ray.origin + t*normalize(ray.direction);
            data.normal = normalize(edge1^edge2);
            data.color = this->material.getColorAt(0, 0);
            data.material = this->material;
            data.distance = t;
            data.exist = true;
            return true;
        }

        // This means that there is a line intersection but not a ray intersection.
        return false;
    }

};

class Plane : public Object {
private:
    double A, B, C, D;
    Material material;
public:

    Plane(const Vector3 &point, const Vector3 &normal) {
        Vector3 norm = normalize(normal);
        A = norm.x;
        B = norm.y;
        C = norm.z;
        D = -A*point.x - B*point.y - C*point.z;

        this->material.setAmbient(0);
        this->material.setDiffuse(1);
        this->material.setSpecular(0);
        this->material.setShininess(1);
        this->material.setReflection(0);
    }

    Plane(const Vector3 &point, const Vector3 &normal, const Material &material) : Plane(point, normal) {
        this->material = material;
        this->material.setAmbient(0);
        this->material.setDiffuse(1);
        this->material.setSpecular(0);
        this->material.setShininess(1);
        this->material.setReflection(0.5);
    }

    sf::Color getPixel(const Vector3 &point3D) const {
        //only special case
        sf::Vector2f point2D(point3D.x, point3D.y);

        return this->material.getColorAt(point3D.x, point3D.y);
    }

    Vector3 getNormal() const {
        return Vector3(this->A, this->B, this->C);
    }

    bool intersect(const Ray &ray, CollisionData &data) const {
        const double dot = this->getNormal()*ray.direction;
        if(dot<EPSILON && dot>-EPSILON)
            return false;

        Vector3 p0 = Vector3(0, 0, -D/C);
        Vector3 p1 = Vector3(1, 0, -(D + A)/C);
        Vector3 p2 = Vector3(1, 1, -(D + A + B)/C);

        Vector3 p01 = p1 - p0;
        Vector3 p02 = p2 - p0;

        double t = -(((p01^p02)*(ray.origin - p0))/(ray.direction*(p01^p02)));

        data.point = ray.origin + t*ray.direction;

        if((ray.direction*(data.point - ray.origin))<EPSILON)
            return false;

        data.normal = this->getNormal();
        data.color = this->getPixel(data.point);
        data.material = this->material;
        data.distance = length(data.point - ray.origin);
        data.exist = true;
        return true;
    }

};

class Torus : public Object {
private:
    double majorRadius, minorRadius;
    Material material;
    Transform3 transform;
public:

    Torus() {
        this->majorRadius = 2;
        this->minorRadius = 1;
    }

    Torus(const Vector3 &center, const double &majorRadius, const double &minorRadius) {
        this->transform.translate(center);

        this->majorRadius = majorRadius;
        this->minorRadius = minorRadius;

        this->material = Material(rand()%255, rand()%255, rand()%255);
    }

    Torus(const Vector3 &center, const double &majorRadius, const double &minorRadius, const Material &material) : Torus(center, majorRadius, minorRadius) {
        this->material = material;
    }

    sf::Color getPixel() const {
        return this->material.getColor();
    }

    Vector3 getNormal(const Vector3 &point) const {
        const double alpha = (this->majorRadius*this->majorRadius)/std::sqrt(point.x*point.x + point.y*point.y);

        return normalize(point - alpha*Vector3(point.x, point.y, 0));
        //return normalize(point - this->majorRadius*normalize(Vector3(point.x, point.y, 0)));
    }

    CollisionData intersect(const Ray &ray) const {
        CollisionData data;
        const Vector3 center = this->transform.getTranslation();
        const Ray rayRelative(ray.origin - center, ray.direction);

        //http://cosinekitty.com/raytrace/chapter13_torus.html
        const double A = this->majorRadius;
        const double B = this->minorRadius;
        const Vector3 D = rayRelative.origin;
        const Vector3 E = rayRelative.direction;
        const double G = 4*A*A*(E.x*E.x + E.y*E.y);
        const double H = 8*A*A*(D.x*E.x + D.y*E.y);
        const double I = 4*A*A*(D.x*D.x + D.y*D.y);
        const double J = E*E;
        const double K = 2*D*E;
        const double L = D*D + (A*A - B*B);

        std::vector<double> tSolutions = solveQuarticEquation(J*J, 2*J*K, 2*J*L + K*K - G, 2*K*L - H, L*L - I);
        //std::vector<double> tSolutions = approxPolynomialRoots({J*J, 2*J*K, 2*J*L + K*K - G, 2*K*L - H, L*L - I}, -100, 100, 1);
        if(tSolutions.size()==0)
            return data;

        double tNearestPositive = 1E+9;
        bool exist = false;
        for(unsigned int i=0; i<tSolutions.size(); i++) {
            if(tSolutions[i]>EPSILON && tSolutions[i]<tNearestPositive) {
                tNearestPositive = tSolutions[i];
                exist = true;
            }
        }
        if(!exist)
            return data;

        data.point = rayRelative.origin + tNearestPositive*normalize(rayRelative.direction);
        data.normal = this->getNormal(data.point-center);
        data.color = this->getPixel();
        data.material = this->material;
        data.distance = length(data.point - rayRelative.origin);
        data.exist = true;

        return data;
    }

    CollisionData distance(const Vector3 &point) const {
        CollisionData data;
        const double R = this->majorRadius;
        const double r = this->minorRadius;
        const Vector3 center = this->transform.getTranslation();

        Vector3 t = point - center;

        data.distance = length(Vector3(length(Vector3(t.x, t.y, 0)) - R, 0, t.z)) - r;
        data.point = point;
        data.normal = this->getNormal(data.point-center);
        data.color = this->material.getColor();
        data.material = this->material;
        data.exist = (data.distance<EPSILON);

        return data;
    }

};

int main() {

    srand(time(NULL));

    sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ray Tracing");

	window.setMouseCursorVisible(false);
	sf::Vector2i center = sf::Vector2i(window.getSize().x, window.getSize().y)/2;
	sf::Mouse::setPosition(center, window);

	View view(Vector3(-40, 20, 20), 1);
	RenderScene scene;
    scene.setRenderMode(SPHERE_TRACING_MODE);

	TextureMenager menager;
	//menager.load("../textures/notexture.jpg");

	Sphere earth(Vector3(0, 20, 20), 7);

    scene.addObject(&earth);
    scene.addObject(new Torus(Vector3(0, 10, 10), 10, 4));
    scene.addObject(new Sphere(Vector3(0, 0, 20), 7));
    //scene.addObject(new Sphere(Vector3(0, 10, 20), 7));
    //scene.addObject(new Sphere(Vector3(0, 40, 20), 7));
    //scene.addObject(new Plane(Vector3(0, 0, 0), Vector3::UnitZ(), Material(menager.getTextureReference(0), 5000, 5000)));

    //scene.addLightSource(new LightSource(Vector3(0, 0, 40)));
    //scene.addLightSource(new LightSource(Vector3(-40, 0, 20)));

    double angle = 0;
    double scroll = 5;

    std::vector<double> sol = solveQuarticEquation(2, -3, 4, -5, -6);

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type==sf::Event::Closed)
                window.close();
            if(event.type==sf::Event::KeyPressed) {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                    window.close();
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
                    scene.setRenderMode(SPHERE_TRACING_MODE);
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                    scene.setRenderMode(RAY_TRACING_MODE);
            }
            if(event.type==sf::Event::MouseWheelScrolled) {
                if(event.mouseWheelScroll.delta>0)  scroll++;
                else                                scroll--;
            }
        }

        sf::Vector2i deltaMouse = sf::Mouse::getPosition(window) - center;
        sf::Mouse::setPosition(center, window);

        scene.setReflectionDepth(3);
        //scene.setRenderResolution(720, 405);
        scene.setRenderResolution(15*scroll, 10*scroll);
        //view.setDistanceFromProjectionPlane(scroll/10);

        viewMove(view, deltaMouse);

		Transform3 earthTransformOriginal = earth.getTransform();
        Transform3 earthTransform = earth.getTransform();
		earthTransform.translate(Vector3(0, 10*std::sin(angle), 0));
        earth.setTransform(earthTransform);

		clock.restart();

		sf::Image frameBuffer = scene.render(view);

		double renderTime = clock.restart().asSeconds();

		earth.setTransform(earthTransformOriginal);
		angle +=0.03;

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
