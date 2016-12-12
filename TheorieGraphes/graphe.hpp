//
//  graphe.hpp
//  TheorieGraphes
//

#ifndef graphe_hpp
#define graphe_hpp

#include <iostream>
#include <stdio.h>
#include <algorithm>
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
<<<<<<< HEAD
    void createMatrix ();
=======
>>>>>>> b4c23b57fa8f0b12eaedcdade466493d8b4b6885

private:
    vector< char > vertex;
    vector< int > cost;
    vector< vector< bool > > adjacent;
    vector< vector< int > > values;
<<<<<<< HEAD
    vector< vector< char > > fmatrix;

=======
    vector< vector< int > > fmatrix;
>>>>>>> b4c23b57fa8f0b12eaedcdade466493d8b4b6885
};

#endif /* graphe_hpp */
