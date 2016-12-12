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

using namespace std;

class Graphe {

public:
    Graphe();
<<<<<<< HEAD


=======
    Graphe(string filepath);
>>>>>>> f8561449cb6b165cbe844bdbb1ef51c899d70906
    // Déstructeur
    void load(string filepath);
    bool addVertex(char name, int cost);
    bool addArc(char origin, char destination);

private:
<<<<<<< HEAD
    int ** adjacent;
    int ** values;
    int ** fmatrix;

=======
    vector< char > vertex;
    vector< int > cost;
    vector< vector< bool > > adjacent;
    vector< vector< int > > values;
    vector< vector< int > > fmatrix;
    
>>>>>>> f8561449cb6b165cbe844bdbb1ef51c899d70906
};

#endif /* graphe_hpp */
