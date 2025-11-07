#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main() {

    // pour remonter d'un dossier (../) pour trouver "data/exemple1.txt"
    const char *fichier_graphe = "../data/exemple_valid_step3.txt";

    //Pour créer le fichier de sortie dans le dossier parent (../)
    const char *fichier_mermaid = "../graph_output.txt";

    // Etape 1
    ListeAdjacence *graphe = readGraph(fichier_graphe);
    afficherListeAdjacence(graphe);

    // Etape 2
    verifierGrapheMarkov(graphe);

    // Etape 3
    if (graphe != NULL) {
        genererFichierMermaid(graphe, fichier_mermaid);
    }

    return 0;
}