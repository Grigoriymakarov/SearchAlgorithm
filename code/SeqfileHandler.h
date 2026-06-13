#ifndef SEQFILEHANDLER_H // sert à empêcher les inclusions multiples d’un même fichier header.
#define SEQFILEHANDLER_H
#include <fstream>
#include <string>
#include "PinfileHandler.h"
#include "dict.h"
#include "AlgoSW.h"
#include <array>

class SeqfileHandler {
    public :

    public :
    // Constructeur
    SeqfileHandler(const std::string& filename, PinfileHandler& pinffile,AlgoSW& algofile);
    // Destructeur
    ~SeqfileHandler() ;
    struct TopItem {
        int newscore;      
        int indiceproteine; };
    const std::array<TopItem, 20>& getTopList() const ; 
    std::array<TopItem, 20> Analysedeproteine(const std::string& prot,const int GOP,const int GEP,const int (&BLOSUM62)[24][24]);
    void closeSeqFile();
    int getindiceprotprelim() const ; 
    void findTheProtein(const std::string& prtRef) ; 
     
    private : 
    int indiceprotprelim ; 
    std::array<TopItem, 20> topList;
    std::ifstream prtFile;
    PinfileHandler &pinfile;
    AlgoSW &algoSW; 
    
  

};
#endif