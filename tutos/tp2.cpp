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

class Sphere : public Objecta
{
    public:
        Point c;
        float rayon;

        Sphere(Point p, float r, Color co) : c(p), rayon(r)
        {couleur = co;};
        Sphere(const Sphere & sph) : c(sph.c), rayon(sph.rayon)
        {couleur = sph.couleur;};
        const Sphere & operator=(const Sphere & sph)
        {
        c = sph.c; rayon = sph.rayon;
        couleur = sph.couleur;
        return *this;};
        bool intersect(Ray ray,Hit &hit)
        {
            std::cout << "Sphere::intersect()" << std::endl;
            float a = dot(ray.direction, ray.direction);
            float b = 2*dot(ray.direction, make_vector(c, ray.origin));
            float c = dot(make_vector(this->c, ray.origin),make_vector(this->c, ray.origin)) - rayon*rayon;
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
};

class Plan : public Objecta
{
    public:
        Point a;
        Vector normal;

        Plan(Point p, Vector n, Color c) : a(p), normal(n)
        {couleur = c;};
        Plan(const Plan & pla) : a(pla.a), normal(pla.normal)
        {couleur = pla.couleur;};
        const Plan & operator=(const Plan & pla)
        {
        a = pla.a; normal = pla.normal;
        couleur = pla.couleur;
        return *this;};
        bool intersect(Ray ray,Hit &hit)
        {
            std::cout << "Plan::intersect()" << std::endl;
            float t;
            t = dot(make_vector(a, ray.origin),normal)/ dot(ray.direction, normal);

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
};



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

void DrawPlane(Plan p)
{

}
void DrawSphere(Sphere s)
{

}

Color getCouleurIntersect(std::vector<Objecta> objs, Ray r){
    float min_hit = FLT_MAX;
    Color intersect_col = objs[0].couleur;
    Hit hit = {{0,0,0}, make_vector({0,0,0}, {0,0,0}), 0};

    for(std::vector<Objecta>::iterator it = objs.begin(); it != objs.end(); it++){
        (*it).intersect(r, hit);
        if(hit.t < min_hit){
            min_hit = hit.t;
            intersect_col = (*it).couleur;
        }
    }
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
    Point a = {0.0f,0.0f,0.0f};
    Point b = {0.5f,0.5f,0.5f};
    Point c = {-1.0f,-1.0f,-1.0f};
    Vector n = {0.5f, 1.0f, 0.0f};
    Plan plan(a, n, make_color(255,255,255));
    Sphere sphere1(a, 0.5, make_color(255,0,0));
    Sphere sphere2(b, 0.5, make_color(0,255,0));
    Sphere sphere3(c, 0.5, make_color(0,0,255));
    std::vector<Objecta> objs;
    objs.push_back(plan);
    objs.push_back(sphere1);
    objs.push_back(sphere2);
    objs.push_back(sphere3);

    for(int y= 0; y < image.height; y++)
    for(int x= 0; x < image.width; x++)
    {
        // generer l'origine et l'extremite du rayon
        Point o = d0 + x*dx0 + y*dy0;
        Point e = {0.0f, 0.0f, 5.0f};
        Ray ray = make_ray(o, e);

        // calculer les intersections
        /*
        Hit hit = {o, make_vector(o, e), 0};
        intersect(plan, ray, hit);
        float tplan = hit.t;

        intersect(sphere1, ray, hit);
        float tsphere1 = hit.t;

        intersect(sphere2, ray, hit);
        float tsphere2 = hit.t;
        */

        image_set_pixel(image, x, y, getCouleurIntersect(objs, ray));
        /*
        if(tplan < 0 && tsphere1 < 0 ){
            image_set_pixel(image, x, y, make_color(0, 0, 0));
        }
        if(tsphere1 > tsphere2){

        }
        if(tplan > tsphere1)
        {
            // calculer une couleur pour le pixel
            //Point p = hit.p + hit.t*hit.n;
            image_set_pixel(image, x, y, sphere1.couleur);
        }
        else
        {
            image_set_pixel(image, x, y, plan.couleur);
        }
        */
    }

    // enregistrer l'image
    write_image(image, "render.png");
    // nettoyage
    release_image(image);
    return 0;
}

