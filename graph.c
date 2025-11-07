#include <stdio.h>
#include <stdlib.h>
#include <string.h> // <--- AJOUTÉ POUR LA CORRECTION
#include "graph.h"


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

// --- DEBUT ETAPE 3 (Corrigée) ---

// On a besoin de "utils.h" pour la fonction getID()
#include "utils.h"

void genererFichierMermaid(ListeAdjacence *graphe, const char *nomFichier) {

    FILE *file = fopen(nomFichier, "wt"); // Ouvre le fichier en mode "write text"
    if (file == NULL) {
        perror("Erreur: Impossible d'ouvrir le fichier de sortie Mermaid");
        return;
    }

    // Écrire l'en-tête de configuration de Mermaid
    fprintf(file, "config:\n");
    fprintf(file, "  layout: elk\n");
    fprintf(file, "  theme: neo\n");
    fprintf(file, "  look: neo\n");
    fprintf(file, "flowchart LR\n");
    fprintf(file, "\n");

    // Étape 1: Déclarer tous les sommets (nœuds)
    for (int i = 1; i <= graphe->taille; i++) {
        // Écrit la ligne, ex: "  A((1))"
        fprintf(file, "  %s((%d))\n", getID(i), i);
    }
    fprintf(file, "\n");

    // Étape 2: Écrire toutes les arêtes (transitions)
    for (int i = 1; i <= graphe->taille; i++) {

        // --- CORRECTION IMPORTANTE ---
        // On doit copier l'ID de départ, car getID() utilise
        // un buffer statique qui sera écrasé par l'appel suivant.
        char idDepart[10]; // Un buffer local pour notre copie
        strcpy(idDepart, getID(i)); // On copie la valeur (ex: "A")
        // --- FIN CORRECTION ---

        // Parcourir la liste chaînée de ce sommet
        Cellule *courant = graphe->listes[i]->head;
        while (courant != NULL) {

            // Obtenir l'ID du sommet d'arrivée
            char *idArrivee = getID(courant->sommet_arrivee);
            float proba = courant->probabilite;

            // Écrit la ligne, ex: "  A --> |0.95| A"
            // On utilise notre copie "idDepart"
            fprintf(file, "  %s --> |%.2f| %s\n", idDepart, proba, idArrivee);

            courant = courant->suivante;
        }
    }

    fclose(file); // Ne pas oublier de fermer le fichier
    printf("Fichier Mermaid '%s' généré avec succès.\n", nomFichier);
}

// --- FIN ETAPE 3 ---