#ifndef PHRFILEHANDLER_H // sert à empêcher les inclusions multiples d’un même fichier header.
#define PHRFILEHANDLER_H

#include <fstream>
#include <string>

#include "PinfileHandler.h"

class PhrfileHandler {
private:
    std::ifstream phrFile;
    PinfileHandler& pin;



public:
    PhrfileHandler(const std::string& filename, PinfileHandler& pinFile);
    ~PhrfileHandler();
     std::string findTheHeader(int proteinIndex) ; // Retourne le header (titre) associé à l’indice de protéine
     void closePhrFile(); 


};

#endif 
