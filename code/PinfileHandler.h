#ifndef PINFILEHANDLER_H // sert à empêcher les inclusions multiples d’un même fichier header.
#define PINFILEHANDLER_H
#include <fstream>
#include <string>
#include <utility>
class PinfileHandler
{

public:
// constructeur
    PinfileHandler(const std::string& filename);
// Destructeur
    ~PinfileHandler() ;
    // Fonctions de la classe
    void prepareSeq() ;
    int preparePhr(int proteinIndex) ;
    std::pair<int, int> giveoffSetLSeq() ;
    int getLongueur_titre() const;
    int getLongueur_data() const;
    int getnombre_proteines() const ;
    void closepinFile();

 private:
 // attributs de la classe
    std::ifstream inFile;
    int Longueur_titre;
    int Longueur_data;
    int nombre_proteines;

};
#endif