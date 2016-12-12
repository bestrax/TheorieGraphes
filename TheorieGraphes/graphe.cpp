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
    this->fmatrix = vector< vector< int > >();
}

Graphe::Graphe(string filepath) {
    this->vertex = vector< char >();
    this->cost = vector< int >();
    this->adjacent = vector< vector< bool > >();
    this->values = vector< vector< int > >();
    this->fmatrix = vector< vector< int > >();
    this->load(filepath);
}

void Graphe::load(string const filepath) {
    ifstream file(filepath);
    int nbVertex;
    char name;
    string line;
    char origin;
    int cost;
    
    if (!file) {
        cout<<"Impossible de lire le fichier : "<<filepath<<endl<<endl;
        return;
    }
    
    file >> nbVertex;
    
    for (int i = 0; i< nbVertex; i++) {
        file >> name;
        file >> cost;
        cout<<name<<" "<<cost<<endl;
        //this->addVertex(name, cost);
    }
    
    while (!file.eof()) {
        file >> line;
        cout<<line<<" "<<line.size()<<endl;
        
        if (line.size() > 2) {
            origin = line[0];
            for (int i = 1; i<line.size(); i++) {
                if(line[i] == '.')
                    break;
                //this->addArc(origin, line[i]);
                cout<<origin<< " " << line[i]<<endl;
            }
        }
    }
    
    
    
}


bool Graphe :: addVertex(char name, int cost) {


    return true;
}
