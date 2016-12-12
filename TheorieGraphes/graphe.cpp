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
    this->fmatrix = vector< vector< string > >();
}

Graphe::Graphe(string filepath) {
    this->vertex = vector< char >();
    this->cost = vector< int >();
    this->adjacent = vector< vector< bool > >();
    this->values = vector< vector< int > >();
    this->fmatrix = vector< vector< string > >();
    this->load(filepath);
    this->createMatrix();
    this->displayMatrix();
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
        if ( this->addVertex(name, cost) == false) {
            cout<<"Mauvaise formation du fichier : multiple declaration du meme sommet"<<endl<<endl;
            return;
        }
    }

    while (!file.eof()) {
        file >> line;

        if (line.size() > 2) {
            origin = line[0];
            for (int i = 1; i<line.size(); i++) {
                if(line[i] == '.')
                    break;
                if ( this->addArc(origin, line[i]) == false) {
                    cout<<"Mauvaise formation du fichier : mauvaise contrainte"<<endl<<endl;
                    return;
                }
            }
        }
    }

}

bool Graphe::addVertex(char name, int cost) {

    if (find(vertex.begin(), vertex.end(), name) != vertex.end())
        return false;

    this->vertex.push_back(name);
    this->cost.push_back(cost);

    return true;
}

bool Graphe::addArc(char origin, char destination) {

    if (this->adjacent.size() == 0) {
        for(int i = 0; i < this->vertex.size(); i++) {
            this->adjacent.push_back(vector< bool > (this->vertex.size(), false));
        }
    }

    if (this->values.size() == 0) {
        for(int i = 0; i < this->vertex.size(); i++) {
            this->values.push_back(vector< int > (this->vertex.size(), 0));
        }
    }

    int posOrigin = find(this->vertex.begin(), this->vertex.end(), origin) - this->vertex.begin();
    int posDestination = find(this->vertex.begin(), this->vertex.end(), destination) - this->vertex.begin();

    if(posOrigin < 0 || posDestination < 0)
        return false;

    this->adjacent[posOrigin][posDestination] = true;
    this->values[posOrigin][posDestination] += this->cost[posOrigin];


    return true;
}

<<<<<<< HEAD
void displayAdj (**adjacent)
{
        cout<<"Matrice d'adjacence"<<endl<<endl<<"\t\t";
    for (int i = 0; i < numberVertex; i++) {
        cout<< this->vertex[i]<< "\t|\t";
    }

    cout<<endl;

    for (int i = 0; i < numberVertex; i++) {
        cout<< this->vertex[i]<< "\t|\t";
        for (int j = 0; j < numberVertex; j++) {
            cout << this->adjacent[i][j] << "\t|\t";
        }
        cout<<endl;

}


void displayValue (**values)
{
       cout<<"Matrice des Valeurs"<<endl<<endl<<"\t\t";
    for (int i = 0; i < numberVertex; i++) {
        cout<< this->vertex[i]<< "\t|\t";
    }

    cout<<endl;

    for (int i = 0; i < numberVertex; i++) {
        cout<< this->vertex[i]<< "\t|\t";
        for (int j = 0; j < numberVertex; j++) {
            cout << this->values[i][j] << "\t|\t";
        }
        cout<<endl;

}

=======
void Graphe::createMatrix() {
    int const numberVertex = (int)this->vertex.size() ;
    int i = 0,j = 0;

    // create table
    for (i = 0; i < numberVertex; i++) {
        this->fmatrix.push_back(vector< string > (numberVertex));
    }

    // initalize fmatrix
    for (i = 0; i < numberVertex; i ++) {
        for(j = 0; j < numberVertex; j ++) {
            if ((this-> adjacent[i][j] == true))
                this->fmatrix[i][j] = to_string(this->values[i][j]);
            else
                this->fmatrix[i][j] = " ";
        }
    }
}

void Graphe::displayMatrix() {
    int const numberVertex = (int)this->vertex.size();

    cout<<"----------------------------"<<endl<<"Graphe d'ordonnancement"<<endl<<"----------------------------"<<endl<<endl;

    cout<<this->vertex.size()<<" sommets"<<endl<<endl;

    cout<<"Matrice d'adjacence et des valeurs"<<endl<<endl<<"\t\t";
    for (int i = 0; i < numberVertex; i++) {
        cout<< this->vertex[i]<< "\t|\t";
    }

    cout<<endl;

    for (int i = 0; i < numberVertex; i++) {
        cout<< this->vertex[i]<< "\t|\t";
        for (int j = 0; j < numberVertex; j++) {
            cout << this->fmatrix[i][j] << "\t|\t";
        }
        cout<<endl;
    }
}
>>>>>>> 1684de4e133a893be3724ad8dab12c307f1474ee

