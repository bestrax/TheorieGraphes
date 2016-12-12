//
//  graphe.hpp
//  TheorieGraphes
//

#ifndef graphe_hpp
#define graphe_hpp

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

class Graphe {
    
public:
    Graphe();
    Graphe(string filepath);
    // Déstructeur
    
    bool addVertex(char name, int cost);
    bool addArc(char origin, char destination);
    
private:
    int ** adjacent;
    int ** values;
    int ** fmatrix;
    
};

#endif /* graphe_hpp */
