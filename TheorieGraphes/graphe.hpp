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
#include <sstream>

using namespace std;

class Graphe {

public:
    Graphe();
    Graphe(string filepath);

    // Déstructeur
    void load(string const filepath);
    bool addVertex(char name, int cost);
    bool addArc(char origin, char destination);
    void createMatrix();
    void displayMatrix();
    void displayAdj();
    void displayValue();
    void computeRank();
    void GanttBegin ();
    void GanttEnd ();
    vector< int > searchRoot(vector< vector< bool > > &adjacent, vector< int > ignore);

private:
    vector< char > vertex;
    vector< int > cost;
    vector< vector< bool > > adjacent;
    vector< vector< int > > values;
    vector< vector< string > > fmatrix;
    vector< int > rank;
    vector< int > dateBegin;
    vector< int > dateEnd;

};

// Afin de régler le problème des vieux GCC (par exemple ceux disponible à l'école...)
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#endif /* graphe_hpp */
