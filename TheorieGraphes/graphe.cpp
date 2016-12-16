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
    this->createMatrix();
    this->displayMatrix();
    this->displayAdj();
    this->displayValue();
    this->transitif();
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

void Graphe::computeRank() {

    int k = 0, deleted = 0;
    vector< int > roots;
    vector< vector< bool > > adjacentMatrix = this->adjacent;

    while (deleted < (this->vertex.size() - 1) ) {
        roots = searchRoot(adjacentMatrix);
        for (int i = 0; i < roots.size(); i++) {
            this->rank[roots[i]] = k;
            deleted++;
            for (int j = 0; j < adjacentMatrix[roots[i]].size(); j++) {
                adjacentMatrix[roots[i]][j] = false;
            }
        }
    }
    cout<<adjacentMatrix.size();
}

vector< int > Graphe::searchRoot(vector< vector< bool > > &adjacent) {

    vector< int > roots;


    for (int i = 0; i < adjacent.size(); i++) {
        bool isRoot = true;
        for (int j = 0; j <adjacent[i].size(); j++) {
            if (adjacent[i][j] == true)
                isRoot = false;
        }

        if (isRoot)
            roots.push_back(i);
    }

    return roots;
}

void Graphe::transitif()
 {
    int const numberVertex = (int)this->vertex.size();
    int i=0;
    int j=0;
    int k=0;

        for (k = 0; k < numberVertex; k++)
        {
            for (i = 0; i < numberVertex; i++)
            {
                for (j = 0; j < numberVertex; j++)
                {
                    if ((this->adjacent[i][k] *this->adjacent[k][j] != 0) && (i != j))
                    {
                        if ((this->adjacent[i][k] + this->adjacent[k][j] < this->adjacent[i][j]) || (this->adjacent[i][j] == 0))
                        {
                            this->adjacent[i][j] =this->adjacent[i][k] + this->adjacent[k][j];
                        }
                    }
                }
            }
     }

     cout<<"Matrice transitive detection de circuit "<<endl<<endl<<"\t\t";
        for ( i = 0; i < numberVertex; i++) {
            cout<< this->vertex[i]<< "\t|\t";
        }

        cout<<endl;

        for ( i = 0; i < numberVertex; i++) {
            cout<< this->vertex[i]<< "\t|\t";
            for (j = 0; j < numberVertex; j++) {
                cout << this->adjacent[i][j] << "\t|\t";
            }
            cout<<endl;
        }

    for ( i = 0 ; i < numberVertex ; i++)
    {
        for (j = 0 ; j < numberVertex ; j++)
        {
            if ((adjacent[i][j] == 1)&&(i==j))
            {
                cout<<" circuit detecte : abandon"<<endl;
                exit(0);
                cout<<"L'abandon n'a pas fonctionne"<<endl;

            }

        }
    }
 }
