#ifndef ALGOSW_H  // sert à empêcher les inclusions multiples d’un même fichier header.
#define ALGOSW_H
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <iostream>
#include <string>

class AlgoSW
{
public : 
// Constructeur
    AlgoSW ();
// Destructeur
    ~AlgoSW() ;

    int smithWatermanAffine(const std::string& query,
                        const std::string& db,
                        int gapOpen,
                        int gapExt,const int (&BLOSUM62)[24][24]) const ; 

private:
    const std::unordered_map<char, int> a_index = {
    {'A', 0}, {'R', 1}, {'N', 2}, {'D', 3}, {'C', 4},
    {'Q', 5}, {'E', 6}, {'G', 7}, {'H', 8}, {'I', 9},
    {'L',10}, {'K',11}, {'M',12}, {'F',13}, {'P',14},
    {'S',15}, {'T',16}, {'W',17}, {'Y',18}, {'V',19},
    {'B',20}, {'Z',21}, {'X',22}, {'*',23}
};
    
    int blosum62_score(char a, char b,const int (&BLOSUM62)[24][24] ) const ;
    
        } ; 


#endif