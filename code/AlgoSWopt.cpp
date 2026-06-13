#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <iostream>
using namespace std;
#include "AlgoSWopt.h"

// Constructeur par défaut
AlgoSWopt::AlgoSWopt() {}

// Destructeur par défaut
AlgoSWopt::~AlgoSWopt() {}



int AlgoSWopt:: blosum62_score(const char a, const char b, const int (&BLOSUM62)[24][24]) const
// Renvoie le score de substitution BLOSUM62 entre deux acides aminés a et b.
{                              // Nous supposons que a_index contient toutes les lettres possibles d'une séquence de protéine.
    const int ia = a_index.at(a);    // récupère l'indice de a dans la matrice  
    const int ib = a_index.at(b);    // récupère l'indice de b dans la matrice 
    return BLOSUM62[ia][ib];
}
// Implémente l'algorithme de Smith-Waterman et renvoie le meilleur score local
int AlgoSWopt:: smithWatermanAffine(const string& query,
                        const string& db,
                        const int gapOpen,
                        const int gapExt,const int (&BLOSUM62)[24][24]) const 
// calcule le meilleur score d’alignement local entre query et db
{
    // Longueurs des séquences
    const int m = static_cast<int>(query.size()); 
    const int n = static_cast<int>(db.size());

    const int NEG_INF = numeric_limits<int>::min() / 4;
    // Initialisation des vecteurs H,E,F et H_prev
    vector<int> H(n + 1, 0);
    vector<int> E(n + 1, NEG_INF);
    vector<int> F(n + 1, NEG_INF);
    vector<int> H_prev(n + 1, 0);

    int best = 0;

    for (int i = 1; i <= m; ++i) {
            H[0] = 0;
            E[0] = NEG_INF;
            F[0] = NEG_INF;
        for (int j = 1; j <= n; ++j) {
            int diag = H_prev[j-1] ; 
            E[j] =max({H[j-1] - gapOpen-gapExt,E[j-1] - gapExt,0});
            F[j] = max({H_prev[j] - gapOpen - gapExt,F[j] - gapExt,0});
            int match = diag + blosum62_score(query[i-1], db[j-1],BLOSUM62);
            H[j] = max({ match, E[j], F[j],0});
            // Met à jour le meilleur score global
            if (H[j] > best) best = H[j];
        }
        H_prev = H;
    }

    return best;
}