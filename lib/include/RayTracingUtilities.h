#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

#include <SFML/Graphics.hpp>

#include "Vector.h"
#include "Matrix.h"

#include "RayTracingMaterial.h"
#include "RayTracingTransform.h"

/*  Ray struct  
    Struct contains origin point and normalized direction vector */

struct Ray {
    Vector3 origin;
    Vector3 direction;

    Ray();
    Ray(const Vector3&, const Vector3&);
};

/*  CollisionData struct
    Struct contains all data requied to calculate reflections and other stuff.
    When method intersect() or distance() returns this struct, it should
    be initialized by user with correct values (like in example).               */

struct CollisionData {
    Vector3 normal;         // normalized normal to surface in specific point
    Vector3 point;          // point of collision
    Material material;      // material of objects where the collision occurred
    sf::Color color;        // nearest collision object color
    double distance;        // distance to the object
    bool exist;             // true if collision occurred

    CollisionData();

    static CollisionData min(const CollisionData&, const CollisionData&);
    static CollisionData smin(const CollisionData&, const CollisionData&, const double&);
};

/*  LightSource class
    It is part of the scene. At this moment describes only position of light point.     */

class LightSource {
private:
    Vector3 position;
public:
    LightSource();
    LightSource(const Vector3&);
    const Vector3 & getPosition() const;
};

/*  Object class
    Class can be drawn on the scene.
    Whene user want to inheriting them form this class
    and overrite one from distance() or intersect() methods (like in example).
    When custom object has only one of this methods,
    then will be appear only in coresponding mode.                                  */

class Object {
protected:
	Material material;
	Transform3 transform;
public:
    virtual ~Object() {};
    virtual CollisionData intersect(const Ray&) const { return CollisionData(); }     // for ray tracing
    virtual CollisionData distance(const Vector3&) const { return CollisionData(); }  // for sphere tracing

	const Transform3 & getTransform() const;
    const Material & getMaterial() const;
    void setTransform(const Transform3&);
    void setMaterial(const Material&);

};

/*  TextureMenager class
    Class not connected with projects exacly,
    but is very handy in care about texture pointers.
    User should use one in program to store textures in one place (like in example)     */

class TextureMenager {
private:
    std::vector<sf::Image> textures;
public:
    TextureMenager();
    void load(const std::string&);
    sf::Image* getTextureReference(const int&);
};

/*  handy funstions for SFML native Color class     */

std::ostream & operator<<(std::ostream&, const sf::Color&);
sf::Color operator*(const double&, const sf::Color&);
sf::Color color_interpolation(const sf::Color&, const sf::Color&, const double&);
