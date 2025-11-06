#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


 //* 1. une fonction pour créer une « cellule »

Cellule* creerCellule(int arrivee, float proba) {

    Cellule* nouvelle = (Cellule*)malloc(sizeof(Cellule));

    if (nouvelle == NULL) {
        printf("Erreur: Probleme d'allocation memoire \n");
        return NULL;
    }

    // Remplit les champs de la cellule
    nouvelle->sommet_arrivee = arrivee;
    nouvelle->probabilite = proba;
    nouvelle->suivante = NULL; // La nouvelle cellule pointe vers rien

    return nouvelle;
}


 //* 2. une fonction pour créer une « liste » vide

Liste* creerListe() {

    Liste* nouvelle = (Liste*)malloc(sizeof(Liste));


    if (nouvelle == NULL) {
        printf("Erreur: Probleme d'allocation memoire pour la liste!\n");
        return NULL;
    }

    nouvelle->head = NULL;

    return nouvelle;
}


 //* 3. une fonction pour ajouter une cellule à une liste

void ajouterCellule(Liste *liste, int arrivee, float proba) {

    Cellule* nouvelleCellule = creerCellule(arrivee, proba);

    nouvelleCellule->suivante = liste->head;
    liste->head = nouvelleCellule;
}


//* 4. une fonction pour afficher une liste
void afficherListe(Liste *liste, int sommet_depart) {

    printf("Liste pour le sommet %d: [head @]", sommet_depart);

    // On part de la tête de la liste
    Cellule *courant = liste->head;

    while (courant != NULL) {
        printf(" -> (%d, %.2f) @", courant->sommet_arrivee, courant->probabilite);

        courant = courant->suivante;
    }
    printf("\n");
}


// * 5. une fonction pour créer une liste d’adjacence ‘vide’

ListeAdjacence* creerListeAdjacence(int taille) {

    ListeAdjacence* graphe = (ListeAdjacence*)malloc(sizeof(ListeAdjacence));
    if (graphe == NULL) {
        printf("Erreur: Probleme d'allocation memoire pour le graphe!\n");
        return NULL;
    }

    graphe->taille = taille;

    graphe->listes = (Liste**)malloc((taille + 1) * sizeof(Liste*));
    if (graphe->listes == NULL) {
        printf("Erreur: Probleme d'allocation memoire pour le tableau de listes!\n");
        return NULL;
    }

    // Crée une liste vide pour chaque sommet
    for (int i = 0; i <= taille; i++) {
        graphe->listes[i] = creerListe();
    }

    return graphe;
}


 //* 6. Une fonction pour afficher une liste d’adjacence

void afficherListeAdjacence(ListeAdjacence *graphe) {
    printf("--- Affichage de la Liste d'Adjacence (Taille: %d) ---\n", graphe->taille);

    // On boucle pour chaque sommet, de 1 à 'taille'
    for (int i = 1; i <= graphe->taille; i++) {
        // On utilise la fonction d'affichage de liste qu'on vient de créer
        afficherListe(graphe->listes[i], i);
    }
    printf("------------------------------------------------------\n");
}
