#ifndef SEQFILEHANDLEROPT_H // sert à empêcher les inclusions multiples d’un même fichier header.
#define SEQFILEHANDLEROPT_H
#include <fstream>
#include <string>
#include <array>
#include "dict.h"
#include "AlgoSWopt.h"
#include "PinfileHandler.h"

class SeqfileHandleropt {

    public :
    // Constructeur
    SeqfileHandleropt(const std::string& filename, PinfileHandler& pinffile,AlgoSWopt& algofile);
    // Destructeur
    ~SeqfileHandleropt() ;
    struct TopItem {
        int newscore;      
        int indiceproteine; };
    const std::array<TopItem, 20>& getTopList() const ; 
    std::array<TopItem, 20> Analysedeproteine(const std::string& prot,const int GOP,const int GEP,const int (&BLOSUM62)[24][24]);
    void closeSeqFile();
     
    private : 

    std::array<TopItem, 20> topList;
    std::ifstream prtFile;
    std::string prtFileName ;
    PinfileHandler &pinfile;
    AlgoSWopt &algoSWopt; 
    
    struct OffsetInfo {
        int start;
        int len;
        int indiceproteine;
    };

  

};
#endif