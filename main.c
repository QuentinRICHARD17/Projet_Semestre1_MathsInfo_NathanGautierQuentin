#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main() {

    const char *fichier_graphe = "data/exemple1.txt";

    ListeAdjacence *graphe = readGraph(fichier_graphe);
    afficherListeAdjacence(graphe);
    verifierGrapheMarkov(graphe);
    
    return 0;
}