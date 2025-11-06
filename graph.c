#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


Cellule* creerCellule(int arrivee, float proba) {

    Cellule* nouvelle = (Cellule*)malloc(sizeof(Cellule));

    if (nouvelle == NULL) {
        printf("Erreur: Probleme d'allocation memoire \n");
        return NULL;
    }

    nouvelle->sommet_arrivee = arrivee;
    nouvelle->probabilite = proba;
    nouvelle->suivante = NULL;

    return nouvelle;
}


Liste* creerListe() {

    Liste* nouvelle = (Liste*)malloc(sizeof(Liste));

    if (nouvelle == NULL) {
        printf("Erreur: Probleme d'allocation memoire\n");
        return NULL;
    }

    nouvelle->head = NULL;

    return nouvelle;
}


void ajouterCellule(Liste *liste, int arrivee, float proba) {

    Cellule* nouvelleCellule = creerCellule(arrivee, proba);

    nouvelleCellule->suivante = liste->head;
    liste->head = nouvelleCellule;
}


void afficherListe(Liste *liste, int sommet_depart) {

    printf("Liste pour le sommet %d: [head @]", sommet_depart);

    Cellule *courant = liste->head;

    while (courant != NULL) {
        printf(" -> (%d, %.2f) @", courant->sommet_arrivee, courant->probabilite);

        courant = courant->suivante;
    }
    printf("\n");
}


ListeAdjacence* creerListeAdjacence(int taille) {

    ListeAdjacence* graphe = (ListeAdjacence*)malloc(sizeof(ListeAdjacence));
    if (graphe == NULL) {
        printf("Erreur: Probleme d'allocation memoire\n");
        return NULL;
    }

    graphe->taille = taille;

    graphe->listes = (Liste**)malloc((taille + 1) * sizeof(Liste*));
    if (graphe->listes == NULL) {
        printf("Erreur: Probleme d'allocation memoire \n");
        return NULL;
    }

    // on crée une liste vide pour chaque sommet
    for (int i = 0; i <= taille; i++) {
        graphe->listes[i] = creerListe();
    }

    return graphe;
}


void afficherListeAdjacence(ListeAdjacence *graphe) {
    printf("--- Affichage de la Liste d'Adjacence (Taille: %d) ---\n", graphe->taille);

    for (int i = 1; i <= graphe->taille; i++) {

        afficherListe(graphe->listes[i], i);
    }
    printf("------------------------------------------------------\n");
}


ListeAdjacence* readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    int nbvert, depart, arrivee;
    float proba;
    ListeAdjacence* graphe; //ligne rajouter

    if (file==NULL)
     {
         perror("Could not open file for reading");
         exit(EXIT_FAILURE);
     }
     // first line contains number of vertices
     if (fscanf(file, "%d", &nbvert) != 1)
     {
         perror("Could not read number of vertices");
         exit(EXIT_FAILURE);
     }


    graphe = creerListeAdjacence(nbvert);//ligne rajouter

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {

        ajouterCellule(graphe->listes[depart], arrivee, proba); //ligne rajouter

    }

    fclose(file);

    return graphe; //ligne rajouter

}
