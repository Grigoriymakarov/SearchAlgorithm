#include <iostream>
#include <string>
#include <fstream>
#include <arpa/inet.h>
#include "PinfileHandler.h"
#include "SeqfileHandler.h"
#include "PhrfileHandler.h"
#include <sstream>
using namespace std;

int main(int argc, char* argv[3]) {
    if (argc != 3) 
    {
        cerr << "Erreur : il manque un/des arguments " << endl;
        return 1 ; 
    }
    else { 
        string sequence;
        
        string base = argv[2] ;
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
        

        PinfileHandler pinfileHandler(fichierpin);
        pinfileHandler.prepareSeq();
        PhrfileHandler phrfileHandler(fichierphr, pinfileHandler);
        AlgoSW algoSW;
        SeqfileHandler seqfileHandler(fichierseq, pinfileHandler,algoSW);
        seqfileHandler.findTheProtein(sequence);
        int index = seqfileHandler.getindiceprotprelim() ;
        string nom = phrfileHandler.findTheHeader(index) ;
        cout << nom << endl ; 


        pinfileHandler.closepinFile() ;
        phrfileHandler.closePhrFile() ;
        seqfileHandler.closeSeqFile() ; 
        }
}
