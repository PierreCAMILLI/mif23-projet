//! \file tuto2.cpp utilisation de mesh pour decrire les sommets d'un ou plusieurs triangles et les dessiner.
#include <math.h>
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

Ray make_ray( const Point& o, const Point& e )
{
    Ray r;
    r.origin= o;
    r.direction= make_vector(o, e);
    return r;
}

struct Hit
{
    Point p;
    Vector n;
    float t;
};

struct Sphere
{
    Point c;
    float rayon;
};

struct Plan
{
    Point a;
    Vector normal;
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
    Vector n = {0.5f, 1.0f, 0.0f};
    Plan plan= {a, n};

    Sphere sphere = {a, 0.5};

    for(int y= 0; y < image.height; y++)
    for(int x= 0; x < image.width; x++)
    {
        // generer l'origine et l'extremite du rayon
        Point o = d0 + x*dx0 + y*dy0;
        Point e = {0.0f, 0.0f, 5.0f};
        Ray ray = make_ray(o, e);

        // calculer les intersections
        Hit hit = {o, make_vector(o, e), 0};
        intersect(plan, ray, hit);
        float tplan = hit.t;
        intersect(sphere, ray, hit);
        float tsphere = hit.t;
        if(tplan < 0 && tsphere < 0 ){
            Color color= make_color(0, 0, 0);
            image_set_pixel(image, x, y, color);
        }
        if(tplan > tsphere)
        {
            // calculer une couleur pour le pixel
            //Point p = hit.p + hit.t*hit.n;
            Color color= make_color(255, 0, 0);
            image_set_pixel(image, x, y, color);
        }
        else
        {
            Color color= make_color(0, 0, 255);
            image_set_pixel(image, x, y, color);
        }
    }

    // enregistrer l'image
    write_image(image, "render.png");
    // nettoyage
    release_image(image);
    return 0;
}

