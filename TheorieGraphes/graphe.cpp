//
//  graphe.cpp
//  TheorieGraphes
//

#include "graphe.hpp"


Graphe::Graphe() {
    this->nbVertex = 0;
    this->adjacent = NULL;
    this->values = NULL;
    this->fmatrix = NULL;
}

Graphe::Graphe(string filepath){
    this->nbVertex = 0;
    this->adjacent = NULL;
    this->values = NULL;
    this->fmatrix = NULL;
    this->load(filepath);
}

void Graphe::load(string filepath){
    
}
