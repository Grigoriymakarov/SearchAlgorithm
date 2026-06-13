#include <iostream>
#include <string>
#include <fstream>
#include <arpa/inet.h>
#include "PinfileHandler.h"
#include "SeqfileHandler.h"
#include "PhrfileHandler.h"
#include <sstream>
using namespace std;

int main(int argc, char* argv[6]) {
    if (argc != 6) 
    {
        cerr << "Erreur : il manque un/des arguments " << endl;
        return 1 ; 
    }
    else { 
        string sequence;
        int BLOSUM62[24][24];
        
        string base = argv[2] ;
        string blos = argv[3];
        int GOP = stoi(argv[4]);
        int GEP = stoi(argv[5]);
        string fichierpin = base + ".pin" ; 
        string fichierseq = base + ".psq" ; 
        string fichierphr = base + ".phr" ; 
 
        ifstream fichierquery(argv[1]) ;
        
        if (!fichierquery) {
            cerr << "Erreur : impossible d’ouvrir le fichier fasta : " << endl;
        }
        else {
            string ligne ;
            bool a = true ;
            while (getline(fichierquery,ligne)) {
                if (a) {
                    a = false ; }
                else {
                    sequence += ligne ; 
                }

            } 
        }
        

    
        ifstream f(blos);

        if (!f) {
            cerr << "Erreur : impossible d’ouvrir le fichier " << blos <<endl;
        }
        else {

        
            string line;
            stringstream ss;

            // Saut des lignes d’en-tête du fichier BLOSUM
            for ( int k = 0 ; k < 7 ; k++) {
            getline(f, line) ; 
            }

            int row = 0;
            // Lecture des 24 lignes de la matrice
            while (getline(f, line) && row < 24) {
                ss.clear();
                ss << line;                 // Première colonne ignorée (lettre de l’acide aminé)
                string trash;
                ss >> trash;

                for (int i = 0; i < 24; i++) {
                    int val;
                    ss >> val;
                    BLOSUM62[row][i] = val;
                }

                row += 1;

                }
        }
        
        PinfileHandler pinfileHandler(fichierpin);
        pinfileHandler.prepareSeq();
        PhrfileHandler phrfileHandler(fichierphr, pinfileHandler);
        AlgoSW algoSW;
        SeqfileHandler seqfileHandler(fichierseq, pinfileHandler,algoSW);

        array<SeqfileHandler::TopItem, 20> topList = seqfileHandler.Analysedeproteine(sequence,GOP,GEP,BLOSUM62);
        for (const SeqfileHandler::TopItem &seqscore : topList) {   
            string header = phrfileHandler.findTheHeader(seqscore.indiceproteine);
            cout << header << " " << seqscore.newscore << endl ;

        }
        pinfileHandler.closepinFile() ;
        phrfileHandler.closePhrFile() ;
        seqfileHandler.closeSeqFile() ; 
}

}
