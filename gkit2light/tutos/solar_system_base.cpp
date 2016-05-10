#include "window.h"

#include "vec.h"
#include "mat.h"
#include "color.h"

#include "mesh.h"
#include "orbiter.h"

#include "wavefront.h"
#include "draw.h"
#include "membre.h"
#include <math.h>
#include <iostream>
Orbiter camera;

Mesh grid;
membre* robo;
membre* boule;
// utilitaire. creation d'une grille / repere.
Mesh make_grid( int size )
{
    Mesh grid= create_mesh(GL_LINES);

    for(int x= 0; x < size; x++)
    {
        float offset = (float) size / 2 - 0.5f ;
        float px= (float) x - offset;
        push_vertex(grid, make_point(px, 0, -offset));
        push_vertex(grid, make_point(px, 0, offset));
    }

    for(int z= 0; z < size; z++)
    {
        float offset = (float) size / 2 - 0.5f ;
        float pz= (float) z - offset;
        push_vertex(grid, make_point(-offset, 0, pz));
        push_vertex(grid, make_point(offset, 0, pz));
    }

    return grid;
}

// creation des objets openGL
int init( )
{
    camera= make_orbiter();
    grid = make_grid(50);
    grid.color = make_black() ;
    std::cout << "Test Constructor" << std::endl;
    robo = new membre();
    boule = new membre();
    std::cout << "Test Read" << std::endl;
    *robo->obj = read_mesh("data/tete.obj");
    *boule->obj = read_mesh("data/boule.obj");
    robo->fils = boule;
    boule->fils = NULL;
    //mesh_normalize(*robo);

    // etat par defaut
    glClearColor(0.2, 0.2, 0.2, 1);           // couleur par defaut de la fenetre

    glClearDepthf(1);                   // profondeur par defaut
    glDepthFunc(GL_LEQUAL);               // ztest, conserver l'intersection la plus proche de la camera
    glEnable(GL_DEPTH_TEST);            // activer le ztest

    glLineWidth(2.5f);                  // epaisseur des lignes de la grille (pixels)

    return 0;   // renvoyer 0 ras, pas d'erreur, sinon renvoyer -1
}

// affichage
int draw( )
{
    // on commence par effacer la fenetre avant de dessiner quelquechose
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // on efface aussi le zbuffer

    // recupere les mouvements de la souris, utilise directement SDL2
    int mx, my;
    unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);

    // deplace la camera
    if(mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
        // tourne autour de l'objet
        orbiter_rotation(camera, mx, my);

    else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
        // approche / eloigne l'objet
        orbiter_move(camera, mx);

    else if(mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
        // deplace le point de rotation
        orbiter_translation(camera, (float) mx / (float) window_width(), (float) my / (float) window_height());

    // affiche la grille / repere
    draw(grid, camera);
    //membre* it = robo;
    //int i = 0;
    //std::cout << "Test Draw" << std::endl;
    //while(it != NULL)
    //{
        //draw(*robo->obj, camera);
        draw(*(robo->fils)->obj, camera);
        //it = robo->fils;
       // i++;
       // std::cout << "Test"  << i << std::endl;

    //}

    //sphere.draw(0, 0, -5);
    return 1;   // on continue, renvoyer 0 pour sortir de l'application
}

// destruction des objets openGL
int quit( )
{
    return 0;   // ras, pas d'erreur
}


int main( int argc, char **argv )
{
    // etape 1 : creer la fenetre
    Window window= create_window(1024, 640);
    if(window == NULL)
        return 1;       // erreur lors de la creation de la fenetre ou de l'init de sdl2

    // etape 2 : creer un contexte opengl pour pouvoir dessiner
    Context context= create_context(window);
    if(context == NULL)
        return 1;       // erreur lors de la creation du contexte opengl

    // etape 3 : creation des objets
    if(init() < 0)
    {
        printf("[error] init failed.\n");
        return 1;
    }

    // etape 4 : affichage de l'application, tant que la fenetre n'est pas fermee. ou que draw() ne renvoie pas 0
    run(window, draw);

    // etape 5 : nettoyage
    quit();
    release_context(context);
    release_window(window);

    return 0;
}
