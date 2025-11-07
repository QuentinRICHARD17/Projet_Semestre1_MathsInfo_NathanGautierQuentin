#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "utils.h"

//Etape 1

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
    ListeAdjacence* graphe;

    if (file==NULL)
    {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", &nbvert) != 1)
    {
        perror("Could not read number of vertices");
        exit(EXIT_FAILURE);
    }


    graphe = creerListeAdjacence(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {

        ajouterCellule(graphe->listes[depart], arrivee, proba);

    }

    fclose(file);

    return graphe;
}


//Etape 2

int verifierGrapheMarkov(ListeAdjacence *graphe) {

    int estMarkov = 1;

    for (int i = 1; i <= graphe->taille; i++) {

        float sommeProbas = 0.0;
        Cellule *courant = graphe->listes[i]->head;

        while (courant != NULL) {
            sommeProbas = sommeProbas + courant->probabilite;
            courant = courant->suivante;
        }

        if (sommeProbas < 0.99 || sommeProbas > 1.0) {

            if (estMarkov == 1) {
                printf("Le graphe n'est pas un graphe de Markov\n");
            }

            printf("  - la somme des probabilités du sommet %d est %.2f\n", i, sommeProbas);

            estMarkov = 0;
        }
    }

    if (estMarkov == 1) {
        printf("Le graphe est un graphe de Markov\n");
    }

    return estMarkov;
}

// ETAPE 3

void genererFichierMermaid(ListeAdjacence *graphe, const char *nomFichier) {

    FILE *file = fopen(nomFichier, "wt");
    if (file == NULL) {
        perror("Erreur: Impossible d'ouvrir le fichier de sortie Mermaid");
        return;
    }

    fprintf(file, "config:\n");
    fprintf(file, "  layout: elk\n");
    fprintf(file, "  theme: neo\n");
    fprintf(file, "  look: neo\n");
    fprintf(file, "flowchart LR\n");
    fprintf(file, "\n");

    // MiniÉtape 1: Déclarer tous les sommets (nœuds)
    for (int i = 1; i <= graphe->taille; i++) {
        fprintf(file, "  %s((%d))\n", getID(i), i);
    }
    fprintf(file, "\n");

    // MiniÉtape 2: Écrire toutes les arêtes (transitions)
    for (int i = 1; i <= graphe->taille; i++) {
        char idDepart[10];
        strcpy(idDepart, getID(i));

        // Parcourir la liste chaînée de ce sommet
        Cellule *courant = graphe->listes[i]->head;
        while (courant != NULL) {

            // Obtenir l'ID du sommet d'arrivée
            char *idArrivee = getID(courant->sommet_arrivee);
            float proba = courant->probabilite;

            fprintf(file, "  %s --> |%.2f| %s\n", idDepart, proba, idArrivee);

            courant = courant->suivante;
        }
    }

    fclose(file);
    printf("Fichier Mermaid '%s' généré avec succès.\n", nomFichier);
}

// --- FIN ETAPE 3 ---