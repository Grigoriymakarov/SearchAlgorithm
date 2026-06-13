#include "PhrfileHandler.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

PhrfileHandler::PhrfileHandler(const string& filename, PinfileHandler& pinffile) : pin(pinffile)
    {

        phrFile.open(filename, ios::binary);
        if (!phrFile.is_open()) {
            cerr << "Erreur : impossible d’ouvrir le fichier .phr : " << filename << endl;
        }
    }

PhrfileHandler::~PhrfileHandler() {
    }
string PhrfileHandler:: findTheHeader(int proteinIndex) 
// Renvoie le "header" (titre) d'une protéine donnée par son index.
{
    phrFile.clear(); // remet à zéro les flags d'erreur (eof, fail)
    int offset = pin.preparePhr(proteinIndex); // On trouve l'offset grâce à l'index de la protéine donnée dans le fichier psq. On va directement à l'offset donnée dans la table des offsets dans le fichier pin.
    phrFile.seekg(offset, ios::beg); // L'offset donnée dans le fichier pin nous permet d'aller directement dans la zone concernée dans le fichier phr
    phrFile.seekg(7, ios::cur) ; // Les 7 premiers octets ne sont pas importants(voir référence sur le gitlab)
    unsigned char byte = 0;
    phrFile.read((char*)&byte, 1); // On lit le prochain octet
    vector<unsigned char> nom_header(128); // 128 est la longueur maximale du titre. C'est donné dans la référence dans le git 
    int value = byte & 0x7F; // ET logique avec 01111111 car on s'intéresse aux 7 bits restants
    if (byte & 0x80) // ET logique avec 10000000. Si le permier bit vaut 0 le test dans le if échoue et on va dans le else
     {
        unsigned int longueur ;
        phrFile.read((char*)&longueur, value); // Les 7 bits restants me donnent la longueur en octets du texte qui m'intéresse
    
        phrFile.read((char*)nom_header.data(), longueur); 
    }
    else {
        phrFile.read((char*)nom_header.data(), value); // Les 7 autres bits contiennent directement le texte qui m'intéresse.
    }

    string titre(nom_header.begin(), nom_header.end()); 
    string vrai_titre ;

    int i = 0;
    while (titre[i] != ' ') { // On remarque que la partie qui nous intéresse est avant le premier espace. Prenons cette partie :
        vrai_titre += titre[i];
        i++;

    }

    return vrai_titre ; 

}
void PhrfileHandler::closePhrFile() {  
        phrFile.close();  
    }
