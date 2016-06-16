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

#define EPSILON 0.000001
#define ALIASING_SPACE 0.05
#define ALIASING_VALUE 2

#define COEFF 0.1
#define SPECULAR_REFLECTION 0.1
#define DIFFUSE_REFLECTION 0.1
#define AMBIENT_REFLECTION 0.1
#define SHININESS 0.1

// Rayon
struct Ray
{
    Point origin;
    Vector direction;
    Color color;
};

// Impact
struct Hit
{
    Point p;
    Vector n;
    float t;
};

// Créé un rayon à partir de deux point dont un point d'origine
Ray make_ray( const Point& o, const Point& e )
{
    Ray r;
    r.origin= o;
    r.direction= make_vector(o, e);
    r.color= make_color(0,0,0);
    return r;
}

// Créé un rayon à partir de deux point dont un point d'origine
Ray make_ray( const Point& o, const Point& e, Color c )
{
    Ray r = make_ray(o,e);
    r.color= c;
    return r;
}

// Sphère
struct Sphere
{
    Transform model;
    Point c;
    float rayon;
    Color couleur;
};

// Créé une sphère à partir d'un centre, d'un rayon, d'une transformation et d'un angle
Sphere make_sphere( const Transform& model, const Point& center, const float radius )
{
    Sphere s;
    s.model= model;
    s.c= center;
    s.rayon= radius;
    s.couleur = make_color(0,0,0);
    return s;
}

// Créé une sphère à partir d'un centre, d'un rayon, d'une transformation, d'un angle et d'une couleur
Sphere make_sphere( const Transform& model, const Point& center, const float radius, const Color c )
{
    Sphere s = make_sphere(model, center, radius);
    s.couleur = c;
    return s;
}

// Plan
struct Plan
{
    Point a;
    Vector normal;
    Color couleur;
};

struct Triangle
{
    Point p1;
    Point p2;
    Point p3;
    Color couleur;
};

struct Carre
{
    Triangle t1;
    Triangle t2;
};

struct Damier
{
    Plan plan;
    std::vector<Carre> carres;
};

Carre make_carre(Point p1, Point p2, Point p3, Color col){
    Triangle t1{p1,p2,p3, col};
    Point p4 = {p3.x+(p2.x-p1.x),p3.y+(p2.y-p1.y),p3.z+(p2.z-p1.z)};
    Triangle t2{p4,p2,p3, col};
    Carre c{t1, t2};
    return c;
};

Carre make_carre(Point p1, Point p2, Point p3){
    return make_carre(p1, p2, p3, make_color(0,0,0));
};

// Vecteur de sphères
std::vector<Sphere> spheres;

// Vecteur de plans
std::vector<Plan> plans;

// Vecteur de carrés
std::vector<Carre> carres;

// Vecteur de carrés
std::vector<Damier> damiers;

// Vecteur de rayons
std::vector<Ray> rayons;

// Ajoute un plan à la figure
void add(Plan plan){
    plans.push_back(plan);
}

// Ajoute une sphère à la figure
void add(Sphere sphere){
    spheres.push_back(sphere);
}

// Ajoute un carré à la figure
void add(Carre carre){
    carres.push_back(carre);
}

// Ajoute un damier à la figure
void add(Damier damier){
    damiers.push_back(damier);
}

// Ajoute un rayon à la figure
void add(Ray r){
    rayons.push_back(r);
}

// p2 et p3 représentent la diagonale du damier
Damier make_damier(Point p1, Point p2, Point p3, unsigned int nb_carre_hor, unsigned int nb_carre_ver, Color coul1, Color coul2){
    Vector normale = cross(make_vector(p1,p2),make_vector(p3,p2));
    Plan plan = {p1, normale, make_color(0,0,0)};
    std::vector<Carre> cases;
    // On créé deux booléens pour s'assurer que les couleurs forment bien un damier quel que soit le nombre de cases
    Point np1, np2, np3;
    bool col_choice = false;
    bool choice;
    for(int i = 0; i < nb_carre_ver; i++){
        col_choice = !col_choice;
        choice = col_choice;
        // On initialise les deux premiers points
        // np1-----*-----
        // |       |
        // |       |
        // |       |
        // np2-----*-----
        np1 = { p1.x + ((p2.x-p1.x)/nb_carre_ver)*i,
                p1.y + ((p2.y-p1.y)/nb_carre_ver)*i,
                p1.z + ((p2.z-p1.z)/nb_carre_ver)*i
            };
        np2 = { np1.x + (p2.x-p1.x)/nb_carre_ver,
                np1.y + (p2.y-p1.y)/nb_carre_ver,
                np1.z + (p2.z-p1.z)/nb_carre_ver
            };
        for(int j = 0; j < nb_carre_hor; j++){
            // On initialise le troisième point
            // np1-----np3---
            // |       |
            // |       |
            // |       |
            // np2-----*-----
            np3 = { np1.x + (p3.x-p1.x)/nb_carre_hor,
                    np1.y + (p3.y-p1.y)/nb_carre_hor,
                    np1.z + (p3.z-p1.z)/nb_carre_hor,
                };
            // On créé le carré
            Carre c = make_carre(np1,np2,np3,(choice ? coul1 : coul2));
            cases.push_back(c);
            // On fait passer les points vers la case suivante
            // *-----np1---
            // |       |
            // |       |
            // |       |
            // *-----np2---
            np2 = {np3.x+(np2.x-np1.x),np3.y+(np2.y-np1.y),np3.z+(np2.z-np1.z)};
            np1 = {np3.x, np3.y, np3.z};
            // On inverse la couleur
            choice = !choice;
        }
    }
    Damier damier = {plan, cases};
    return damier;
}

// Indique si le rayon croise le plan
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

void translate(Point &p, Vector v){
    p.x += v.x;
    p.y += v.y;
    p.z += v.z;
}

void translate(Carre &c, Vector v){
    Triangle t = c.t1;
    translate(t.p1,v);
    translate(t.p2,v);
    translate(t.p3,v);
}

void translate(Damier &d, Vector v){
    for(std::vector<Carre>::iterator it = d.carres.begin(); it != d.carres.end(); it++){
        translate((*it), v);
    }
    translate(d.plan.a,v);
}

// Indique si le rayon croise la sphère
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
    /*
    Vector v= {ray.origin.x-sphere.c.x,ray.origin.y-sphere.c.y,ray.origin.z-sphere.c.z};
    float t1 = -dot(v, ray.origin)
    + sqrt(dot(v, ray.origin)*dot(v, ray.origin)
    - ((v.x*v.x+v.y*v.y+v.z*v.z)-r)
    * ((v.x*v.x+v.y*v.y+v.z*v.z)-r));
    float t2 = -dot(v, ray.origin) - sqrt(dot(v, ray.origin)*dot(v, ray.origin) - ((v.x*v.x+v.y*v.y+v.z*v.z)-r) * ((v.x*v.x+v.y*v.y+v.z*v.z)-r));
    ttemp = (t1 < t2) ? t1 : t2;
    if(hit.t > ttemp){
        hit.t = ttemp;
        hit.n = ray.direction;
        hit.p = ray.origin;
        return true;
    }
    */
}
float aire(Point A, Point B, Point C)
{
    float AB =  length(make_vector(A,B));
    float AC =  length(make_vector(A,C));
    float BC =  length(make_vector(C,B));
    float pABC = (AB+AC+BC)/2;
    float aABC = sqrtf(pABC*(pABC - AB)*(pABC - AC)*(pABC - BC));
    return aABC;
}

bool intersect(Triangle triangle, Ray ray, Hit &hit){

    // Find vectors for two edges sharing triangle.p1
    Vector edge1 = make_vector(triangle.p2,triangle.p1);
    Vector edge2 = make_vector(triangle.p3,triangle.p1);

    // Begin calculating determinant - also used to calculate U parameter
    Vector pvec = cross(ray.direction, edge2);

    // If determinant is near zero, ray lies in plane of triangle
    float det = dot(edge1, pvec);

    if(det > -EPSILON && det < EPSILON)
        return false;
    float inv_det = 1/det;

    // Calculate distance from triangle.p1 to ray origin
    Vector tvec = make_vector(ray.origin, triangle.p1);

    // Calculate U parameter and test bounds
    float u = dot(tvec,pvec)*inv_det;
    if(u < 0.0 || u > 1.0)
        return 0;

    // Prepare to test V parameter
    Vector qvec = cross(tvec,edge1);

    // Calculate V parameter and test bounds
    float v = dot(ray.direction, qvec) * inv_det;
    if(v < 0.0 || u+v > 1.0)
        return false;

    // Calculate T, ray intersects triangle
    float ttemp = dot(edge2,qvec) * inv_det;
    if(hit.t > -ttemp){
        hit.t = -ttemp;
        //std::cout << hit.t << std::endl;
        hit.n = ray.direction;
        hit.p = ray.origin;
        return true;
    }
    //std::cout << hit.t << std::endl;

    return false;
}

bool intersect(Carre carre, Ray ray, Hit &hit){
    if(intersect(carre.t1,ray,hit)){
        return true;
    }
    if(intersect(carre.t2,ray,hit)){
        return true;
    }
    return false;
}

// Indique si le rayon croise quelque chose
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

// Retourne la couleur de l'objet touché par le rayon
Color getCouleurIntersect(Ray r, Hit &hit){
    hit.t = FLT_MAX;
    Color intersect_col = make_color(0,0,0);
    // Obtention de la couleur pour contact avec une sphère
    for(std::vector<Sphere>::iterator it = spheres.begin(); it != spheres.end(); it++){

        Sphere s = (*it);

        // Le rayon intercepte la sphère
        if(intersect(s, r, hit)){
            //// Ombrage
            // Point d'intersection entre le rayon et la sphère
            Point croix = hit.p + hit.t*hit.n;
            // Calcul de la normale
            Vector normale = make_vector(s.c, croix);
            // Calcul du cosinus de l'angle entre le rayon et la normal de la sphère
            float coeff = fabs(dot(normalize(r.direction),normalize(normale)));
            // On créé la couleur
            intersect_col = s.couleur * COEFF + s.couleur * r.color * coeff;


            // Calcul du spéculaire
            float dt = dot(hit.n, normale);
            dt = (dt < 0 ? 0 : dt);
            dt = (dt > 1 ? 1 : dt);
            Vector R = 2 * fabs(dot(hit.n, normale)) * hit.n - normale;
            // std::cout << mV.x << ", " << mV.y << ", " << mV.z << std::endl;
            Vector V = normalize(make_vector(r.origin, croix));

            float cof = fabs(dot(R,V));
            Color specular = r.color * powf(cof,8);

            intersect_col = intersect_col + specular;

            //std::cout << hit.t << std::endl;
        }
    }
    // Obtention de la couleur pour contact avec un plan
    for(std::vector<Plan>::iterator it = plans.begin(); it != plans.end(); it++){
        Plan p = (*it);
        if(intersect(p, r, hit)){
            // Si le rayon est dans la distance de visionnage
            float coeff = fabs(dot(normalize(r.direction),normalize(p.normal)));
            intersect_col = p.couleur * COEFF + p.couleur * r.color * coeff;
            //std::cout << hit.t << std::endl;
        }
    }
    // Obtention de la couleur pour contact avec un carré
    for(std::vector<Carre>::iterator it = carres.begin(); it != carres.end(); it++){
        Carre c = (*it);
        if(intersect(c, r, hit)){
            // Si le rayon est dans la distance de visionnage
            Triangle triangle = c.t1;
            Vector normal = cross(make_vector(triangle.p2, triangle.p1),make_vector(triangle.p3, triangle.p1));
            float coeff = fabs(dot(normalize(r.direction),normalize(normal)));
            intersect_col = triangle.couleur * COEFF + triangle.couleur  * r.color * coeff;
        }
    }
    // Obtention de la couleur pour contact avec un damier
    Damier dhit;
    bool touched = false;
    Hit thit = hit;
    for(std::vector<Damier>::iterator it = damiers.begin(); it != damiers.end(); it++){
        Damier d = (*it);
        if(intersect(d.plan, r, thit)){
            dhit = d;
            touched = true;
        }
    }
    if(touched){
        for(std::vector<Carre>::iterator it = dhit.carres.begin(); it != dhit.carres.end(); it++){
            Carre c = (*it);
            if(intersect(c, r, hit)){
                // Si le rayon est dans la distance de visionnage
                Triangle triangle = c.t1;
                Vector normal = cross(make_vector(triangle.p2, triangle.p1),make_vector(triangle.p3, triangle.p1));
                float coeff = fabs(dot(normalize(r.direction),normalize(normal)));
                intersect_col = triangle.couleur * COEFF + triangle.couleur  * r.color * coeff;
                hit = thit;
                break;
            }
        }
    }
    return intersect_col;
}

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

int main( int agc, char **argv )
{
    Orbiter camera = make_orbiter();
    // creer l'image resultat
    Image image= create_image(1024, 640, 3, make_color(0, 0, 0));
    Point d0;
    Vector dx0, dy0;
    orbiter_image_frame(camera, image.width, image.height,0,  45.0f, d0, dx0, dy0);
    std::cout << d0.x << ", " << d0.y << ", " << d0.z << "." << std::endl;
    std::cout << dx0.x << ", " << dx0.y << ", " << dx0.z << "." << std::endl;
    std::cout << dy0.x << ", " << dy0.y << ", " << dy0.z << "." << std::endl;


    // creer les objets de la scene
    Point a = {0.0f,0.0f,0.0f};
    Point b = {0.5f,0.5f,0.5f};
    Point c = {-1.0f,-1.0f,-1.0f};
    Vector n = {0.0f, 100.0f, 1.0f};
    Plan plan{a, n, make_color(1.0f,1.0f,1.0f)};
    Sphere sphere1{make_identity(), a, 0.75, make_color(1.0f,0.0f,1.0f)};
    Sphere sphere2{make_identity(), b, 0.75, make_color(0.0f,1.0f,0.0f)};
    Sphere sphere3{make_identity(), c, 0.75, make_color(0.0f,0.0f,1.0f)};
    //Carre carre1 = make_carre({-1.0f,1.0f,2.0f}, {-1.0f,0.0f,2.0f}, {0.0f,1.0f,2.0f}, make_color(1,0,0));
    //make_damier({-1.0f,1.0f,1.0f},{-1.0f,0.0f,1.0f},{0.0f,1.0f,3.0f},4,4,make_color(1.0f,1.0f,1.0f),make_color(0.0f,0.0f,0.0f));
    //make_damier({-1.0f,1.0f,1.0f},{-1.0f,0.0f,3.0f},{0.0f,1.0f,1.0f},4,4,make_color(1.0f,1.0f,1.0f),make_color(0.0f,0.0f,0.0f));
    /*
    Damier damier1 = make_damier(   {-1.5f,1.0f,0.0f},  // Point en haut à gauche
                                    {-1.5f,-1.0f,9.5f}, // Point en bas à gauche
                                    {1.5f,1.0f,0.0f},   // Point en haut à droite
                9,9,make_color(1.0f,1.0f,1.0f),make_color(0.0f,0.0f,0.0f));
    */
    Damier damier1 = make_damier(   {-0.5f,0.3f,4.0f},  // Point en haut à gauche
                                    {-0.5f,-0.3f,5.0f}, // Point en bas à gauche
                                    {0.5f,0.3f,4.0f},   // Point en haut à droite
                9,9,make_color(1.0f,1.0f,1.0f),make_color(0.0f,0.0f,0.0f));
    //translate(damier1, {2,0.25,-0.1});
    add(plan);
    add(sphere1);
    add(sphere2);
    add(sphere3);
    add(damier1);
//    add(carre1);

    for(int y= 0; y < image.height; y++)
    {
      std::cout<<"*"<<std::flush;
      for(int x= 0; x < image.width; x++)
      {
        /*
          Point o = d0 + x*dx0 + y*dy0;
          Point e = {0.0f, 0.0f, 5.0f};
          Ray ray = make_ray(o, e);

          Hit hit;
          image_set_pixel(image, x, y, getCouleurIntersect(ray, hit));
          */

          // generer l'origine et l'extremite du rayon

          float xmin = x-ALIASING_SPACE;
          float ymin = y-ALIASING_SPACE;
          float r = 0;
          float g = 0;
          float b = 0;
          for(int k = 0; k < ALIASING_VALUE; k++)
          {
            float newX = RandomFloat(xmin, xmin+(2*ALIASING_SPACE));
            float newY = RandomFloat(ymin, ymin+(2*ALIASING_SPACE));
            Point o = d0 + newX*dx0 + newY*dy0;
            Point e = {0.0f, 0.0f, 5.0f};
            Ray ray = make_ray(o, e, make_color(1.0,0.0,1.0));

            Hit hit;
            Color intersectCol = getCouleurIntersect(ray, hit);
            r+=intersectCol.r;
            g+=intersectCol.g;
            b+=intersectCol.b;
          }
          Color pix = make_color(r/(float)ALIASING_VALUE, g/(float)ALIASING_VALUE, b/(float)ALIASING_VALUE);

          image_set_pixel(image, x, y, pix);
      }
      //std::cout<<"\n"<<std::flush;
        // multiplier par l'angle compris entre la surface et le rayon
    }
    //Anti-aliasing spacial
    /*
    for(int y= 1; y < image.height-1; y++)
      for(int x= 1; x < image.width-1; x++)
      {
          Color tmp = image_pixel(image, x, y);
          float r = tmp.r;
          float g = tmp.g;
          float b = tmp.b;
          for(int k = -1; k < 2; k++)
            for(int l = -1; l < 2; l++)
          {
            Color voisin = image_pixel(image, x+k, y+l);
            r+=voisin.r;
            g+=voisin.g;
            b+=voisin.b;
          }
          Color pix = make_color(r/9.0f, g/9.0f, b/9.0f);
          image_set_pixel(image, x, y, pix);
      }
    */
    // enregistrer l'image
    write_image(image, "render.png");
    // nettoyage
    release_image(image);
    return 0;
}

