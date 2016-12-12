//
//  graphe.cpp
//  TheorieGraphes
//

#include "graphe.hpp"


Graphe::Graphe() {
    this->adjacent = NULL;
    this->values = NULL;
    this->fmatrix = NULL;
}

Graphe::Graphe(string filepath){
    this->adjacent = NULL;
    this->values = NULL;
    this->fmatrix = NULL;
    
}
