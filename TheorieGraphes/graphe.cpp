//
//  graphe.cpp
//  TheorieGraphes
//

#include "graphe.hpp"


Graphe::Graphe() {
    this->vertex = vector< char >();
    this->cost = vector< int >();
    this->adjacent = vector< vector< bool > >();
    this->values = vector< vector< int > >();
    this->fmatrix = vector< vector< char > >();
}

Graphe::Graphe(string filepath){
    this->vertex = vector< char >();
    this->cost = vector< int >();
    this->adjacent = vector< vector< bool > >();
    this->values = vector< vector< int > >();
    this->fmatrix = vector< vector< char > >();
    this->load(filepath);
}

void Graphe::load(string filepath){

}


void createMatrix (){
    int numberVertex = this->vertex.size() + 2 ;
    int i = 0,j = 0;

 // create table
    for (i = 0;i < numberVertex; i++)
    {
        this->fmatrix.push_back(vector< char > (numberVertex));
    }

// initalize fmatrix
    for (i = 0;i < numberVertex;i ++)
    {
        for(j = 0;j < numberVertex;j ++)
        {
            if ((this-> adjacent[i][j] == 1) || i == 0 || j == 0)
                this->fmatrix[i][j] = this->values[i][j];
            else
                this->fmatric[i][j] = ' ';
        }
    }

// print fmatrix
    for(i = 0;i < numberVertex;i ++)
    {
        for(j = 0;j < numberVertex;j ++)
        {
            std :: cout << this->fmatrix[i][j] << "|";
        }
        std :: cout << "\n" ;
    }
}
