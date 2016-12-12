//
//  graphe.hpp
//  TheorieGraphes
//

#ifndef graphe_hpp
#define graphe_hpp

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Graphe {
    
public:
    Graphe();
    Graphe(string filepath);
    // Déstructeur
    void load(string const filepath);
    bool addVertex(char name, int cost);
    bool addArc(char origin, char destination);
    
private:
    vector< char > vertex;
    vector< int > cost;
    vector< vector< bool > > adjacent;
    vector< vector< int > > values;
    vector< vector< int > > fmatrix;
    
};

#endif /* graphe_hpp */
