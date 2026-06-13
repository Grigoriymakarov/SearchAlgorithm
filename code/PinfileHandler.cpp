#include "PinfileHandler.h"
#include <iostream>
#include <arpa/inet.h>
using namespace std;

// Constructeur : ouvre le fichier en lecture binaire
PinfileHandler::PinfileHandler(const string& filename)
{
    inFile.open(filename, ios::binary);
    if (!inFile.is_open()) {
        cout << "impossible d’ouvrir le fichier .pin : " << filename << endl;
    } 
}

// Destructeur par défaut
PinfileHandler::~PinfileHandler(){

}
// se positionner dans le fichier au début de la table d'offsets de séquence
void PinfileHandler::prepareSeq() {
    if (inFile.is_open()) {
            inFile.seekg(8, ios::beg); // Avance de 8 octets depuis le début du fichier
            inFile.read((char *)&Longueur_titre, 4);  // Lit un entier 32 bits : Longueur_titre
            Longueur_titre = ntohl(Longueur_titre); // On passe du format big-endian au format little_endian


            inFile.seekg(Longueur_titre, ios::cur); // Saute le titre (1 caractère ASCII = 1 octet)
            inFile.read((char *)&Longueur_data, 4); // Lit Longueur_data à 32 bits
            Longueur_data = ntohl(Longueur_data); // On passe du format big-endian au format little_endian
            inFile.seekg(Longueur_data, ios::cur);

            inFile.read((char *)&nombre_proteines, 4); // Lit nombre_proteines à 32 bits
            nombre_proteines = ntohl(nombre_proteines);

            inFile.seekg(12 + (nombre_proteines+1)*4, ios::cur);  //On passe le nombres de résidus + la taille de la plus grande protéine + la table d'offsets des headers.
        } else 
            cout << "file.pin is not opened" <<  endl;

}

// se positionner dans le fichier au début de la table d'offsets des headers, similaire à prepareSeq
int PinfileHandler::preparePhr(int proteinIndex) {
    if (inFile.is_open()) {
            inFile.seekg(8, ios::beg); 
            inFile.read((char *)&Longueur_titre, 4); 
            Longueur_titre = ntohl(Longueur_titre);


            inFile.seekg(Longueur_titre, ios::cur);
            inFile.read((char *)&Longueur_data, 4);
             Longueur_data = ntohl(Longueur_data);
            inFile.seekg(Longueur_data, ios::cur);

            inFile.read((char *)&nombre_proteines, 4);
            nombre_proteines = ntohl(nombre_proteines);
            
            inFile.seekg(12, ios::cur); // à partir de maintenant, on rentre dans la table des offsets.
            inFile.seekg(4*proteinIndex,ios::cur) ; // Se déplace dans la table d'offsets : chaque offset = 4 octets
            uint32_t offset;
            inFile.read((char *)&offset, 4);
            return ntohl(offset); // On renvoit l'offset
            
        } 
        else cout << "file.pin is not opened" << endl;
        return 1 ; 

}

pair<int, int> PinfileHandler::giveoffSetLSeq() { // retourne l'offset sur lequel on est maintenant + le nombre d´octets à lire pour une sequence
        uint32_t offset1;
        uint32_t offset2;
        // Lit deux offsets consécutifs
        inFile.read((char *)&offset1, 4);
        inFile.read((char *)&offset2, 4);
        // On recule de 4 octets : on se replace sur offset2
        inFile.seekg(-4, ios::cur);
        // On retourne : (offset de départ, taille du bloc)
        return {ntohl(offset1), ntohl(offset2) - ntohl(offset1)};
}

void PinfileHandler:: closepinFile() {
     if (inFile.is_open()) {
        inFile.close();
    }
}
// Getters :
    int PinfileHandler::getLongueur_titre() const {
        return Longueur_titre;
    }
    int PinfileHandler::getLongueur_data() const {
        return Longueur_data;
    }
    int PinfileHandler::getnombre_proteines() const {
        return nombre_proteines;
    }