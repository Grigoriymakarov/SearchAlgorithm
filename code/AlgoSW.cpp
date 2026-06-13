#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
#include "AlgoSW.h"


AlgoSW::AlgoSW() {}

  

AlgoSW::~AlgoSW() {}



int AlgoSW:: blosum62_score(const char a, const char b, const int (&BLOSUM62)[24][24]) const
{
    // Renvoie le score de substitution BLOSUM62 entre deux acides aminés a et b.
                                // Nous supposons que a_index contient toutes les lettres possibles d'une séquence de protéine.
    const int ia = a_index.at(a);     // récupère l'indice de a dans la matrice
    const int ib = a_index.at(b);     // récupère l'indice de b dans la matrice
    return BLOSUM62[ia][ib];
}
// Implémente Smith-Waterman
int AlgoSW:: smithWatermanAffine(const string& query,
                        const string& db,
                        int gapOpen,
                        int gapExt,const int (&BLOSUM62)[24][24]) const 
{
    const int m = static_cast<int>(query.size());
    const int n = static_cast<int>(db.size());

    const int NEG_INF = std::numeric_limits<int>::min() / 4;

    vector<vector<int>> H(m+1, vector<int>(n+1, 0));
    vector<vector<int>> E(m+1, vector<int>(n+1, NEG_INF));
    vector<vector<int>> F(m+1, vector<int>(n+1, NEG_INF));

    int best = 0;

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {

            // Gap horizontal
            E[i][j] =
                max({H[i][j-1] - gapOpen-gapExt,
                E[i][j-1] - gapExt,0});

            // Gap vertical
            F[i][j] = max({
                H[i-1][j] - gapOpen - gapExt,
                F[i-1][j] - gapExt,0});

            int match = H[i-1][j-1] + blosum62_score(query[i-1], db[j-1],BLOSUM62);

            H[i][j] = max({ match, E[i][j], F[i][j]});

            if (H[i][j] > best) best = H[i][j];
        }
    }

    return best;
}