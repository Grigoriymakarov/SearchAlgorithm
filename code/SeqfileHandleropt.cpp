#include "SeqfileHandleropt.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "dict.h"
#include <omp.h>
#include <limits>
using namespace std;

// Constructeur
SeqfileHandleropt::SeqfileHandleropt(const string& filename, PinfileHandler& pinffile,AlgoSWopt& algofile)
    : pinfile(pinffile),algoSWopt(algofile)
    {
        prtFile.open(filename, ios::binary);
        prtFileName = filename ;
    if (!prtFile.is_open()) {
        cerr << "Erreur : impossible d’ouvrir le fichier .psq : " << filename << endl;
    }
}
// Destructeur
SeqfileHandleropt:: ~SeqfileHandleropt() {
}


// Compare une séquence de référence prtRef à toutes les protéines de la base de données
// en utilisant l'algorithme de Smith-Waterman avec pénalités affines (GOP/GEP).
// Retourne les 20 meilleurs scores avec l’indice de la protéine correspondante.
array<SeqfileHandleropt::TopItem, 20>
SeqfileHandleropt::Analysedeproteine(const string& prtRef,
                               const int GOP,
                               const int GEP,
                               const int (&BLOSUM62)[24][24])
{  
    vector<OffsetInfo> offsets; 
    offsets.reserve(pinfile.getnombre_proteines());
    for (int indiceprot = 0; indiceprot < pinfile.getnombre_proteines(); ++indiceprot) {
        pair<int, int> offsetL = pinfile.giveoffSetLSeq(); // giveoffSetLSeq() renvoie (start, len) pour la protéine courante
        offsets.push_back({ offsetL.first, offsetL.second, indiceprot });
    }
    // Initialisation du tableau
    array<TopItem, 20> topList;
    for ( TopItem &item : topList) {
        item.newscore   = numeric_limits<int>::min();
        item.indiceproteine = -1;
    }
    
// les blocs de 64 protéines sont distribués dynamiquement entre threads
    #pragma omp parallel for schedule(dynamic, 64)
    for (int i = 0; i < (int)(offsets.size()); ++i) {
        const OffsetInfo &off = offsets[i];
        // Chaque thread ouvre son propre ifstream
        ifstream localFile(prtFileName, ios::binary);
        if (!localFile) {
            continue; 
        }

        localFile.seekg(off.start, ios::beg); // On se place au début de la séquence de protéine

        vector<unsigned char> protein(off.len);
        localFile.read((char*)(protein.data()), off.len);

        int proteine_longueur = off.len;
        // Si proteine_longueur se termine par 0x00 (caractère nul), on retire ce caractère.
        if (proteine_longueur > 0 && protein.back() == 0x00) {
            --proteine_longueur;
        }
        string seq;
        // On passe du format binaire au format normal (alphabétique) :
        seq.resize(proteine_longueur);
        for (int k = 0; k < proteine_longueur; ++k) {
            seq[k] = dict.at((int)(protein[k]));
        }
        // Calcul du score d’alignement local avec l'algorithme de Smith-Waterman entre la protéine de référence prtRef et la séquence seq.
        int newscore = algoSWopt.smithWatermanAffine(
            prtRef,
            seq,
            GOP,
            GEP,
            BLOSUM62
        );
        // Comme topList est partagé entre threads, on passe en section critique pour éviter la concurrence entre threads. 
        #pragma omp critical
        {
            // On cherche la position où insérer newscore
            int pos = 0;
            while (pos < 20 &&
        (newscore < topList[pos].newscore ||
        (newscore == topList[pos].newscore &&
        off.indiceproteine > topList[pos].indiceproteine)))
        {
        ++pos;
        }
            if (pos < 20) {
                for (int j = 19; j > pos; --j) {
                    topList[j] = topList[j - 1];
                }
                topList[pos] = {newscore, off.indiceproteine };
            }
        }
    }

    return topList;
}


void SeqfileHandleropt::closeSeqFile() {  
        prtFile.close();  
    }
