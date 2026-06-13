#include "SeqfileHandler.h"
#include <fstream>
using namespace std;

// Constructeur
SeqfileHandler::SeqfileHandler(const string& filename, PinfileHandler& pinffile,AlgoSW& algofile)
    : pinfile(pinffile),algoSW(algofile)
    {
        prtFile.open(filename, ios::binary);
    if (!prtFile.is_open()) {
        
}
    }

// Destructeur
SeqfileHandler::~SeqfileHandler() {
    closeSeqFile();
}
// Compare une séquence de référence prtRef à toutes les protéines de la base de données
// en utilisant l'algorithme de Smith-Waterman avec pénalités affines (GOP/GEP).
// Retourne les 20 meilleurs scores avec l’indice de la protéine correspondante.

array<SeqfileHandler::TopItem, 20> SeqfileHandler:: Analysedeproteine(const string& prtRef,const int GOP,const int GEP, const int (&BLOSUM62)[24][24]) {
    // Initialisation de TopList.
    for (TopItem &item : topList) {
        item.newscore = 0 ;
        item.indiceproteine = -1;
    }
        // Parcours de toutes les protéines de la base de données
        for (int indiceprot = 0; indiceprot < pinfile.getnombre_proteines(); ++indiceprot) {
            pair<int, int> offsetL = pinfile.giveoffSetLSeq();
            int start = offsetL.first;
            int len   = offsetL.second;
            prtFile.seekg(start, ios::beg);
            
            // Si proteine_longueur se termine par 0x00 (caractère nul), on retire ce caractère.
            vector<unsigned char> protein(len);
            prtFile.read((char*)protein.data(), len);
            int protein_length = protein.size();
            if ( protein.back() == 0x00) {
            protein_length -= 1;
            }
            // On passe du format binaire au format normal (alphabétique) :
            string seq;
            seq.resize(protein_length);
            for (int k = 0; k < protein_length; ++k) {
                seq[k] = seq[k] = dict.at((int)(protein[k]));
        }     
         // Calcul du score d’alignement local avec l'algorithme de Smith-Waterman entre la protéine de référence prtRef et la séquence seq.
        int newscore = algoSW.smithWatermanAffine(prtRef,seq,GOP,GEP,BLOSUM62) ;  
        int i = 0;
        
        // Insertion du score dans le top 20 avec un tri décroissant et , si égalité au niveau des scores, on garde l'ordre dans la base de donnée.
        while  (i < 20 && 
       (newscore < topList[i].newscore || 
        (newscore == topList[i].newscore && 
         indiceprot > topList[i].indiceproteine)))  {
    i++;
}

        if (i != 20) {

            for (int j = 19; j > i; j--) {
                topList[j] = topList[j - 1];
            }
            topList[i] = {newscore,indiceprot};
        }
        
    }
    return topList ;
}


void SeqfileHandler:: findTheProtein(const string& prtRef) {
        for (indiceprotprelim = 0; indiceprotprelim < pinfile.getnombre_proteines(); ++indiceprotprelim) {
            pair<int, int> offsetL = pinfile.giveoffSetLSeq();
            int start = offsetL.first;
            int len   = offsetL.second;
            prtFile.seekg(start, ios::beg);
            
            // Si proteine_longueur se termine par 0x00 (caractère nul), on retire ce caractère.
            vector<unsigned char> protein(len);
            prtFile.read((char*)protein.data(), len);
            int protein_length = protein.size();
            if ( protein.back() == 0x00) {
            protein_length -= 1;
            }
            // On passe du format binaire au format normal (alphabétique) :
            string seq;
            seq.resize(protein_length);
            for (int k = 0; k < protein_length; ++k) {
                seq[k] = dict.at((int)protein[k]);
        }    
            if (seq == prtRef) {

                
                return ;
        }
}
}

int SeqfileHandler::getindiceprotprelim() const {
    return indiceprotprelim ; 
}
void SeqfileHandler::closeSeqFile() {  
        prtFile.close();  
    }
