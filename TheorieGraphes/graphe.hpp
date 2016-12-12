//
//  graphe.hpp
//  TheorieGraphes
//

#ifndef graphe_hpp
#define graphe_hpp

#include <stdio.h>

class Graphe {
    
public:
    Graphe();
    // Déstructeur
    bool addVertex(char name, int cost);
    bool addArc(char origin, char destination);
    
private:
    int ** adjacent;
    int ** values;
    int ** fmatrix;
    
};

#endif /* graphe_hpp */
