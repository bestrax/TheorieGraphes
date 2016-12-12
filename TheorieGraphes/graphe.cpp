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

Graphe::Graphe(string filepath) {
    this->vertex = vector< char >();
    this->cost = vector< int >();
    this->adjacent = vector< vector< bool > >();
    this->values = vector< vector< int > >();
    this->fmatrix = vector< vector< char > >();
    this->load(filepath);
}

<<<<<<< HEAD
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
=======
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
>>>>>>> b4c23b57fa8f0b12eaedcdade466493d8b4b6885
}
