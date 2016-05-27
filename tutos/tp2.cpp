//! \file tuto2.cpp utilisation de mesh pour decrire les sommets d'un ou plusieurs triangles et les dessiner.
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <float.h>
#include "window.h"
#include "mesh.h"
#include "image.h"
#include "vec.h"
#include "mat.h"
#include "orbiter.h"

//std::vector<Object *> scene;

struct Ray
{
    Point origin;
    Vector direction;
};

struct Hit
{
    Point p;
    Vector n;
    float t;
};

class Objecta {
    public:
        Color couleur;

        Objecta(){};

        virtual bool intersect(Ray ray,Hit &hit){
        std::cout << "Objecta::intersect()" << std::endl;
        return false;};
};

Ray make_ray( const Point& o, const Point& e )
{
    Ray r;
    r.origin= o;
    r.direction= make_vector(o, e);
    return r;
}

struct Sphere
{
    Transform model;
    Point c;
    float rayon;
    Color couleur;
};

Sphere make_sphere( const Transform& model, const Point& center, const float radius )
{
    Sphere s;
    s.model= model;
    s.c= center;
    s.rayon= radius;
    s.couleur = make_color(0,0,0);
    return s;
}

Sphere make_sphere( const Transform& model, const Point& center, const float radius, const Color c )
{
    Sphere s = make_sphere(model, center, radius);
    s.couleur.r = c.r;
    s.couleur.g = c.g;
    s.couleur.b = c.b;
    s.couleur.a = c.a;
    return s;
}

struct Plan
{
    Point a;
    Vector normal;
    Color couleur;
};

std::vector<Sphere> spheres;

std::vector<Plan> plans;

void add(Plan plan){
    plans.push_back(plan);
}

void add(Sphere sphere){
    spheres.push_back(sphere);
}

bool intersect(Plan plan,Ray ray,Hit &hit)
{
    float t;
    t = dot(make_vector(plan.a, ray.origin),plan.normal)/ dot(ray.direction, plan.normal);

    if(t < 0)
        return false;
    else
    {
        if(hit.t > t){
            hit.t = t;
            hit.n = ray.direction;
            hit.p = ray.origin;
            return true;
        }
        else return false;
    }
}

bool intersect(Sphere sphere,Ray ray,Hit &hit)
{
    float a = dot(ray.direction, ray.direction);
    float b = 2*dot(ray.direction, make_vector(sphere.c, ray.origin));
    float c = dot(make_vector(sphere.c, ray.origin),make_vector(sphere.c, ray.origin)) - sphere.rayon*sphere.rayon;
    float delta = b*b - 4*a*c;
    if(delta <= 0)
        return false;
    else
    {
        float t1 = (-b + sqrtf(delta))/(2*a);
        float t2 = (-b - sqrtf(delta))/(2*a);
        float ttemp;
        ttemp = (t1 < t2) ? t1 : t2;
        if(hit.t > ttemp){
            hit.t = ttemp;
            hit.n = ray.direction;
            hit.p = ray.origin;
            return true;
        }
        return false;
    }
}

bool intersect(Ray ray,Hit &hit){
    for(std::vector<Sphere>::iterator it = spheres.begin(); it != spheres.end(); it++){
        if(intersect((*it), ray, hit))
            return true;
    }
    for(std::vector<Plan>::iterator it = plans.begin(); it != plans.end(); it++){
        if(intersect((*it), ray, hit))
            return true;
    }
    return false;
}

void DrawPlane(Plan p)
{

}
void DrawSphere(Sphere s)
{

}

Color getCouleurIntersect(Ray r, Hit &hit, Point source){
    float min_hit = FLT_MAX;
    Color intersect_col = plans[0].couleur;
    Hit tmpHit = {{0,0,0}, make_vector({0,0,0}, {0,0,0}), 0};
    bool isSphere = false;
    Point csphere;
    for(std::vector<Sphere>::iterator it = spheres.begin(); it != spheres.end(); it++){
        if(intersect((*it), r, hit))
        if(hit.t < min_hit){
            min_hit = hit.t;
            hit = tmpHit;
            intersect_col = (*it).couleur;
            isSphere = true;
            csphere = (*it).c;
        }
    }
    for(std::vector<Plan>::iterator it = plans.begin(); it != plans.end(); it++){
        if(intersect((*it), r, hit))
        if(hit.t < min_hit){
            min_hit = hit.t;
            hit = tmpHit;
            intersect_col = (*it).couleur;
            isSphere = false;
        }
    }
    // Calcul pour l'ombre
    Vector v;
    if(isSphere){
        v = make_vector(csphere, hit.p);
        //float coeff = cos(dot(normalize(r.direction),normalize(v)));
        float coeff = cos(dot(normalize(r.direction),normalize(v)));
        //std::cout << coeff << std::endl;
        intersect_col = intersect_col*coeff;
    }
    /*
    Ray s_intersect = make_ray(hit.p, source);
    if(intersect(s_intersect, hit))
    {
        intersect_col = intersect_col/2.0f;

    }
    */
    return intersect_col;
}

int main( int agc, char **argv )
{
    Orbiter camera = make_orbiter();
    // creer l'image resultat
    Image image= create_image(1024, 640, 3, make_color(0, 0, 0));
    Point d0;
    Vector dx0, dy0;
    orbiter_image_frame(camera, image.width, image.height,0,  45.0f, d0, dx0, dy0);


    // creer les objets de la scene
    Point source = {-5.0f,-5.0f,-5.0f};
    Point a = {0.0f,0.0f,0.0f};
    Point b = {0.5f,0.5f,0.5f};
    Point c = {-1.0f,-1.0f,-1.0f};
    Vector n = {0.5f, 1.0f, 0.0f};
    Plan plan{a, n, make_color(1,1,1)};
    Sphere sphere1{make_identity(), a, 0.5, make_color(1,0,1)};
    Sphere sphere2{make_identity(), b, 0.5, make_color(0,1,0)};
    Sphere sphere3{make_identity(), c, 0.5, make_color(0,0,1)};
    add(plan);
    add(sphere1);
    add(sphere2);
    add(sphere3);

    for(int y= 0; y < image.height; y++)
    for(int x= 0; x < image.width; x++)
    {
        // generer l'origine et l'extremite du rayon
        Point o = d0 + x*dx0 + y*dy0;
        Point e = {0.0f, 0.0f, 5.0f};
        Ray ray = make_ray(o, e);

        Hit hit;
        image_set_pixel(image, x, y, getCouleurIntersect(ray, hit, source));
        // multiplier par l'angle compris entre la surface et le rayon
    }

    /*
    Point source = {0.0f,3.0f,0.0f};
    Point a = {-3.0f,0.0f,10.0f};
    Point b = {-1.0f,2.0f,15.0f};
    Point c = {0.0f,0.0f,12.0f};
    Point pl = {0.5f,-1.5f,12.0f};
    Vector n = {0.0f, 1.0f, 0.0f};
    Plan plan{a, n, make_color(1,1,1)};
    Sphere sphere1{make_identity(), a, 0.4, make_color(1,0,1)};
    Sphere sphere2{make_identity(), b, 0.5, make_color(0,1,0)};
    Sphere sphere3{make_identity(), c, 0.2, make_color(0,0,1)};
    add(plan);
    add(sphere1);
    add(sphere2);
    add(sphere3);

    for(int y= 0; y < image.height; y++)
    for(int x= 0; x < image.width; x++)
    {
        // generer l'origine et l'extremite du rayon
        Point o = d0 + x*dx0 + y*dy0;
        Point e = {0.0f, 0.0f, 5.0f};
        Ray ray = make_ray(o, e);

        Hit hit;
        image_set_pixel(image, x, y, getCouleurIntersect(ray, hit, source));
        // multiplier par l'angle compris entre la surface et le rayon
    }
    */

    // enregistrer l'image
    write_image(image, "render.png");
    // nettoyage
    release_image(image);
    return 0;
}

