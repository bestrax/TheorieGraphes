//
//  graphe.cpp
//  TheorieGraphes
//

#include "graphe.hpp"
#include <fstream>


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
    this->displayAdj();
    this->displayValue();
    this->createMatrix();
    this->displayMatrix();
    this->computeRank();
    this->computeDateBegin();
    
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

    cout<<"----------------------------"<<endl<<"Probleme d'ordonnancement"<<endl<<"----------------------------"<<endl<<endl;
    cout<<"Durees des taches"<<endl<<endl;

    for(int i = 0; i <nbVertex; i++) {
        cout<<this->vertex[i]<<"\t";
    }

    cout<<endl;

    for(int i = 0; i <nbVertex; i++) {
        cout<<this->cost[i]<<"\t";
    }

    cout<<endl<<endl;

    cout<<"Contraintes"<<endl<<endl;

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
                cout<<origin<<" ne peut commencer que lorsque la tache "<<line[i]<<" est terminee"<<endl;
            }
        }
    }

    cout<<endl<<endl;

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

void Graphe :: displayAdj ()//vector<char>& adjacent
{
    int const numberVertex = (int)this->vertex.size() ;
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
}

void Graphe :: displayValue ()
{
     int const numberVertex = (int)this->vertex.size() ;
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
}


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
                this->fmatrix[i][j] = patch::to_string(this->values[i][j]);
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

void Graphe::computeDateBegin() {
    
    vector< int > current;
    int calculated = 0, costRank = 0, costBegin = 0;
    const int max = *max_element(this->rank.begin(), this->rank.end());
    int min = max, k= -1;
    
    this->dateBegin = vector< int >(this->vertex.size());
    
    while (calculated < this->vertex.size()) {
        
        min = max;
        costRank = 0;
        for (int i = 0; i < this->rank.size(); i++) {
            if (this->rank[i] > k && this->rank[i] < min) {
                
                current.clear();
                min = this->rank[i];
                current.push_back(i);
                costRank = this->cost[i];
                
            } else if (this->rank[i] == min) {
                
                current.push_back(i);
                if (this->cost[i] > costRank) {
                    costRank = this->cost[i];
                }
                
            }
        }
        
        if (current.size() > 0) {
            k = this->rank[current[0]];
        }
        
        for (int i = 0; i < current.size(); i++) {
            this->dateBegin[current[i]] = costBegin;
        }
        
        costBegin += costRank;
        
        calculated += current.size();
        current.clear();
    }
}

void Graphe::computeRank() {
    
    int k = 0;
    vector< int > roots;
    vector< vector< bool > > adjacentMatrix = this->adjacent;
    vector< int > ignore;
    
    if (this->rank.size() <= this->vertex.size() ) {
        this->rank = vector< int >(this->vertex.size());
    }
    
    while (ignore.size() < this->vertex.size() ) {
        roots = searchRoot(adjacentMatrix, ignore);
        for (int i = 0; i < roots.size(); i++) {
            this->rank[roots[i]] = k;
            ignore.push_back(roots[i]);
        }
        roots.clear();
        k++;
    }
}
vector< int > Graphe::searchRoot(vector< vector< bool > > &adjacentMatrix, vector< int > &ignore) {
    
    vector< int > roots;
    
    for (int i = 0; i < adjacentMatrix.size(); i++) {
        bool isRoot = true;
        if (find (ignore.begin(), ignore.end(), i) == ignore.end()) {
            for (int j = 0; j <adjacentMatrix[i].size(); j++) {
                if (adjacentMatrix[j][i] == true && find (ignore.begin(), ignore.end(), j) == ignore.end())
                    isRoot = false;
            }
            
            if (isRoot)
                roots.push_back(i);
        }
    }

    return roots;
}

