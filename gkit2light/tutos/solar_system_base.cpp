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

membre* tete;
membre* boule1;
membre* torse;

membre* main1;
membre* poignet1;
membre* avantbras1;
membre* bras1;
membre* boule2;

membre* main2;
membre* poignet2;
membre* avantbras2;
membre* bras2;
membre* boule3;

membre* pied1;
membre* tibia1;
membre* cuisse1;
membre* boule4;

membre* pied2;
membre* tibia2;
membre* cuisse2;
membre* boule5;
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
    // Partie tete
    tete = new membre();
    boule1 = new membre();
    torse = new membre();
    // Partie main gauche
    main1 = new membre();
    poignet1 = new membre();
    avantbras1 = new membre();
    bras1 = new membre();
    boule2 = new membre();
    // Partie main droite
    main2 = new membre();
    poignet2 = new membre();
    avantbras2 = new membre();
    bras2 = new membre();
    boule3 = new membre();
    // Partie pied gauche
    pied1 = new membre();
    tibia1 = new membre();
    cuisse1 = new membre();
    boule4 = new membre();
    // Partie pied droit
    pied2 = new membre();
    tibia2 = new membre();
    cuisse2 = new membre();
    boule5 = new membre();

    std::cout << "Test Read" << std::endl;
    // Partie tete
    *tete->obj = read_mesh("data/tete.obj");
    *boule1->obj = read_mesh("data/boule.obj");
    *torse->obj = read_mesh("data/torse.obj");
    // Partie main gauche
    *main1->obj = read_mesh("data/main.obj");
    *poignet1->obj = read_mesh("data/poignet.obj");
    *avantbras1->obj = read_mesh("data/avant_bras.obj");
    *bras1->obj = read_mesh("data/bras.obj");
    *boule2->obj = read_mesh("data/boule.obj");
    // Partie main droite
    *main2->obj = read_mesh("data/main.obj");
    *poignet2->obj = read_mesh("data/poignet.obj");
    *avantbras2->obj = read_mesh("data/avant_bras.obj");
    *bras2->obj = read_mesh("data/bras.obj");
    *boule3->obj = read_mesh("data/boule.obj");
    // Partie pied gauche
    *pied1->obj = read_mesh("data/pied.obj");
    *tibia1->obj = read_mesh("data/tibia.obj");
    *cuisse1->obj = read_mesh("data/cuisse.obj");
    *boule4->obj = read_mesh("data/boule.obj");
    // Partie pied droit
    *pied2->obj = read_mesh("data/pied.obj");
    *tibia2->obj = read_mesh("data/tibia.obj");
    *cuisse2->obj = read_mesh("data/cuisse.obj");
    *boule5->obj = read_mesh("data/boule.obj");

    std::cout << "Test Linking Members" << std::endl;
    // Partie tete
    tete->parent = boule1;
    boule1->parent = torse;
    torse->parent = NULL;
    // Partie main gauche
    main1->parent = poignet1;
    poignet1->parent = avantbras1;
    avantbras1->parent = bras1;
    bras1->parent = boule2;
    boule2->parent = torse;
    // Partie main droite
    main2->parent = poignet2;
    poignet2->parent = avantbras2;
    avantbras2->parent = bras2;
    bras2->parent = boule3;
    boule3->parent = torse;
    // Partie pied gauche
    pied1->parent = tibia1;
    tibia1->parent = cuisse1;
    cuisse1->parent = boule4;
    boule4->parent = torse;
    // Partie pied droit
    pied2->parent = tibia2;
    tibia2->parent = cuisse2;
    cuisse2->parent = boule5;
    boule5->parent = torse;

    std::cout << "Test Translate" << std::endl;
    // Partie tete
    tete->translate(0.1, 3.28, 0);
    boule1->translate(0.1, 3.28, 0);
    torse->translate(0, 0, 0);
    // Partie main gauche
    main1->translate(0.1, 2.66, 3.66);
    poignet1->translate(0.1, 2.66, 3.5);
    avantbras1->translate(0.1, 2.66, 2.25);
    bras1->translate(0.1, 2.66, 1);
    boule2->translate(0.1, 2.66, 1);

    // Partie main droite
    main2->translate(0.1, 2.66, -3.66);
    main2->rotateX(180);
    poignet2->translate(0.1, 2.66, -3.5);
    poignet2->rotateX(180);
    avantbras2->translate(0.1, 2.66, -2.25);
    avantbras2->rotateX(180);
    bras2->translate(0.1, 2.66, -1);
    bras2->rotateX(180);
    boule3->translate(0.1, 2.66, -1);

    // Partie pied gauche
    pied1->translate(0, -2.9, 0.6);
    tibia1->translate(0, -1.53, 0.6);
    cuisse1->translate(0, 0, 0.6);
    boule4->translate(0, 0, 0.6);
    boule4->rescale(1.5, 1.5, 1.5);
    // Partie pied droit
    pied2->translate(0, -2.9, -0.6);
    tibia2->translate(0, -1.53, -0.6);
    cuisse2->translate(0, 0, -0.6);
    boule5->translate(0, 0, -0.6);
    boule5->rescale(1.5, 1.5, 1.5);
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
    for(membre* memb = tete; memb != NULL; memb = memb->parent){
        draw(*memb->obj, memb->getTransformation(), camera);
    }
    for(membre* memb = main1; memb != NULL; memb = memb->parent){
        draw(*memb->obj, memb->getTransformation(), camera);
    }
    for(membre* memb = main2; memb != NULL; memb = memb->parent){
        draw(*memb->obj, memb->getTransformation(), camera);
    }
    for(membre* memb = pied1; memb != NULL; memb = memb->parent){
        draw(*memb->obj, memb->getTransformation(), camera);
    }
    for(membre* memb = pied2; memb != NULL; memb = memb->parent){
        draw(*memb->obj, memb->getTransformation(), camera);
    }
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
