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

struct task {
    int dateBegin;
    int time;
};

class Graphe {

public:
    Graphe();
    Graphe(string filepath);
    void displayAdj(vector< vector< bool > > adjacent);
    void displayValue(vector< vector< int > > values);

    // Déstructeur
    void load(string const filepath);
    bool addVertex(char name, int cost);
    bool addArc(char origin, char destination);
    void createMatrix();
    void displayMatrix();
    void computeRank();
    vector< int > searchRoot(vector< vector< bool > > &adjacent);

private:
    vector< char > vertex;
    vector< int > cost;
    vector< vector< bool > > adjacent;
    vector< vector< int > > values;
    vector< vector< string > > fmatrix;
    vector< int > rank;
    vector< task > tasks;
};

#endif /* graphe_hpp */
