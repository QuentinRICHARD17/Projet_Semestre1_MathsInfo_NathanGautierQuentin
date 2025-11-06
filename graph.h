#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdio.h>

typedef struct Cellule {
    int sommet_arrivee;
    float probabilite;
    struct Cellule *suivante;
} Cellule;

typedef struct Liste {
    Cellule *head;
} Liste;

typedef struct ListeAdjacence {
    Liste **listes;
    int taille;
} ListeAdjacence;


Cellule* creerCellule(int arrivee, float proba);

Liste* creerListe();

void ajouterCellule(Liste *liste, int arrivee, float proba);

void afficherListe(Liste *liste, int sommet_depart);

ListeAdjacence* creerListeAdjacence(int taille);

void afficherListeAdjacence(ListeAdjacence *graphe);

void libererListeAdjacence(ListeAdjacence *graphe);

ListeAdjacence* readGraph(const char *filename);

#endif