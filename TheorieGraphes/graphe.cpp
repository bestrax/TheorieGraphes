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
    this->configGraph();
    this->displayAdj();
    this->displayValue();
    this->createMatrix();
    this->displayMatrix();
    this->computeRank();
    this->computeDateBegin();
    this->computeDateEnd();
    this->GanttBegin();
    this->GanttEnd();

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
    
    if (nbVertex > 0) {
        this->addVertex('a', 0);
    }

    for (int i = 0; i< nbVertex; i++) {
        file >> name;
        file >> cost;
        if ( this->addVertex(name, cost) == false) {
            cout<<"Mauvaise formation du fichier : multiple declaration du meme sommet"<<endl<<endl;
            return;
        }
    }
    
    if (nbVertex > 0) {
        this->addVertex('z', 0);
    }

    cout<<"----------------------------"<<endl<<"Probleme d'ordonnancement"<<endl<<"----------------------------"<<endl<<endl;
    cout<<"Durees des taches"<<endl<<endl;

    for(int i = 0; i <nbVertex+2; i++) {
        cout<<this->vertex[i]<<"\t";
    }

    cout<<endl;

    for(int i = 0; i <nbVertex+2; i++) {
        cout<<this->cost[i]<<"\t";
    }

    cout<<endl<<endl;

    cout<<"Contraintes"<<endl<<endl;

    while (!file.eof()) {
        file >> line;

        if (line.size() >= 2) {
            origin = line[0];
            for (int i = 1; i<line.size(); i++) {
                if(line[i] == '.')
                    break;
                if (this->addArc(line[i], origin) == false) {
                    cout<<"!!! Création de circuit avec valeur potentiellement positive. Contrainte ignoree et arc supprime. ("<<origin<<" ne peut commencer que lorsque la tache "<<line[i]<<" est terminee)"<<endl;
                } else {
                    cout<<origin<<" ne peut commencer que lorsque la tache "<<line[i]<<" est terminee"<<endl;
                }
                
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
    
    if (hasCircuit(posOrigin) == true) {
        this->adjacent[posOrigin][posDestination] = false;
        return false;
    } else {
        this->values[posOrigin][posDestination] += this->cost[posOrigin];
    }

    return true;
}

bool Graphe::hasCircuit(int check, int pos) {
    
    vector< int > current;
    
    if (pos == check)
        return true;
    
    if (pos == -1) {
        pos = check;
    }
    
    for (int i = 0; i < this->vertex.size(); i++) {
        if (this->adjacent[pos][i] == true) {
            current.push_back(i);
        }
    }
    
    for (int i = 0; i < current.size(); i++) {
        if (hasCircuit(check, current[i]) == true) {
            return true;
        }
    }
    
    return false;
    
}

void Graphe::configGraph() {
    
    cout<<"Creation du graphe\n----------------------\nPropriétés du graphe :"<<endl<<endl;
    
    cout<<"- nombre de sommets : "<< patch::to_string(this->vertex.size()-2) <<endl;
    cout<<"- graphe oriente"<<endl;
    cout<<"- 1 valeur (numerique) pour chaque arc"<<endl;
    cout<<"- maximum 1 arc d'un sommet X donne vers un sommet Y donné"<<endl;
    cout<<"--> Ajout des sommets \"debut de projet\" ('a') et \"fin de projet\" ('z')"<<endl;
    cout<<"--> Recherche des points d'entree et points de sortie - Ajout des arcs incidents au debut et à la fin de projet"<<endl;
    this->addExtremity();
    
    cout<<"--> La matrice d’adjacence  : Madj[x][y] = vrai si arc de x vers y"<<endl;
    cout<<"\tde la matrice des valeurs : Mval[x][y] = valeur de l'arc s'il existe"<<endl;
    cout<<"--> Arcs associés aux contraintes de type \"X ne peut commencer que lorsque Y est terminee\""<<endl;
    
    for (int i = 0; i < this->values.size(); i++) {
        
        for (int j = 0; j < this->values[i].size(); j++) {
            
            if (this->values[i][j] != 0) {
                cout<<this->vertex[i]<<" --["<<this->values[i][j]<<"]--> "<<this->vertex[j]<<endl;
            }
            
        }
        
    }
    
    cout<<endl<<endl;
    
}

void Graphe::addExtremity() {
    
    bool entry, exit;
    
    //Rechercher des entrées
    for (int i = 0; i < this->vertex.size(); i++) {
        
        //Si c'est l'entrée ou la sortie on passe
        if (this->vertex[i] == 'a' || this->vertex[i] == 'z') {
            continue;
        }
        entry = true;
        
        //On parcourt toutes les contraintes pouvant mener à ce sommet
        for(int j = 0; j < this->vertex.size(); j++) {
            if (this->values[j][i] != 0) {
                entry = false;
            }
        }
        
        //Si on n'a pas trouvé de contrainte alors on peut ajouter un arc
        if (entry) {
            this->addArc('a', this->vertex[i]);
        }
    }
    
    for (int i = 0; i < this->vertex.size(); i++) {
        if (this->vertex[i] == 'a' || this->vertex[i] == 'z') {
            continue;
        }
        
        exit = true;
        
        for(int j = 0; j < this->vertex.size(); j++) {
            if (this->values[i][j] != 0) {
                exit = false;
            }
        }
        
        if (exit) {
            this->addArc(this->vertex[i], 'z');
        }
    }
    
}

void Graphe :: displayAdj ()
{
    int const numberVertex = (int)this->vertex.size();
    cout<<"Matrice d'adjacence\n----------------------"<<endl<<endl<<"\t\t";
    
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
    
    cout<<endl;
}

void Graphe :: displayValue ()
{
    int const numberVertex = (int)this->vertex.size() ;
    cout<<"Matrice des Valeurs\n----------------------"<<endl<<endl<<"\t\t";
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
    
    cout<<endl;
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

    cout<<this->vertex.size()<<" sommets"<<endl;
    cout<<this->vertex.size()-2<<" taches"<<endl<<endl;

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
    const int numberVertex = (int)this->vertex.size();
    int min = max, k= -1, costTask;

    this->dateBegin = vector< int >(numberVertex);
    
    cout<<endl<<"----------------------------------------------------\n Calcul du calendrier au plus tôt\n----------------------------------------------------"<<endl<<endl;

    while (calculated < numberVertex) {

        min = max;
        costRank = 0;
        for (int i = 0; i < this->rank.size(); i++) {
            if (this->rank[i] > k && this->rank[i] < min) {

                current.clear();
                min = this->rank[i];
                current.push_back(i);

            } else if (this->rank[i] == min) {

                current.push_back(i);
                if (this->cost[i] > costRank) {
                }

            }
        }

        if (current.size() > 0) {
            k = this->rank[current[0]];
        }
        
        cout<<"Calcul pour le rang "<<patch::to_string(k)<<" comprenant les sommets suivants :"<<endl;
        

        for (int i = 0; i < current.size(); i++) {
            
            for (int j =0; j < numberVertex; j++) {
                if (this->adjacent[j][current[i]] == true) {
                    
                    costTask = this->dateBegin[j] + this->cost[j];
                    
                    if (this->dateBegin[current[i]] < costTask) {
                        this->dateBegin[current[i]] = costTask;
                    }
                }
            }
            
            cout<<"- "<<this->vertex[current[i]]<<" commencant a la date "<<this->dateBegin[current[i]]<<endl;
            
        }

        costBegin += costRank;
        
        cout<<"Date de fin du rang : "<<patch::to_string(costBegin)<<endl;

        calculated += current.size();
        current.clear();
    }
}

void Graphe::computeDateEnd() {
    
    if (dateBegin.size() == 0) {
        printf("Merci d effectuer le calcul de date au plus cours au paravant de la date au plus tard");
        return;
    }
    
    vector< int > current;
    int calculated = 0;
    const int min = *min_element(this->rank.begin(), this->rank.end());
    int max = min, k= *max_element(this->rank.begin(), this->rank.end())+1;
    bool lastElement = true;
    
    this->dateEnd = vector< int >(this->vertex.size());
    
    cout<<endl<<"----------------------------------------------------\n Calcul du calendrier au plus tard\n----------------------------------------------------"<<endl<<endl;
    
    while (calculated < this->vertex.size()) {
        
        max = min;
        for (int i = 0; i < this->rank.size(); i++) {
            if (this->rank[i] < k && this->rank[i] > max) {
                current.clear();
                max = this->rank[i];
                current.push_back(i);
                
            } else if (this->rank[i] == max) {
                current.push_back(i);
            }
        }
        
        if (current.size() > 0) {
            k = this->rank[current[0]];
        }
        
            
        if (lastElement) {
            
            const int timeEnd = *max_element(this->dateBegin.begin(), this->dateBegin.end());
            cout<<"Calcul pour le rang "<<patch::to_string(k)<<" comprenant les sommets suivants :"<<endl;
            
            for (int i = 0; i < current.size(); i++) {
                this->dateEnd[current[i]] = timeEnd;
                cout<<"- "<<this->vertex[current[i]]<<" terminant a la date "<<patch::to_string(timeEnd)<<endl;
            }
            lastElement = false;
            
        } else {
            
            int minSuc = 0;
            bool firstSuc;
            
            cout<<"Calcul pour le rang "<<patch::to_string(k)<<" comprenant les sommets suivants :"<<endl;
            
            for (int i = 0; i < current.size(); i++) {
                
                firstSuc = true;
                
                for (int j = 0; j < this->adjacent[current[i]].size(); j++) {
                    
                    if (this->adjacent[current[i]][j] == true) {
                        
                        if (firstSuc) {
                            minSuc = this->dateEnd[j];
                            firstSuc = false;
                        } else if (this->dateEnd[j] < minSuc) {
                            minSuc = this->dateEnd[j];
                        }
                        
                    }
                    
                }
                this->dateEnd[current[i]] = minSuc - this->cost[current[i]];
                cout<<"- "<<this->vertex[current[i]]<<" terminant a la date "<<patch::to_string(this->dateEnd[current[i]])<<endl;
            }
            
            
        }
        
        
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

void Graphe::GanttBegin(){
    int i = 0, date_begin = 0, date_end = 0, j = 0;
    int nb_vertex = (int)this->vertex.size();
    int max_time = *max_element(this->dateBegin.begin(), this->dateBegin.end());
    
    max_time += this->cost[distance(this->dateBegin.begin(), max_element(this->dateBegin.begin(), this->dateBegin.end()))];
    
    cout<<endl<<endl<<"Dates au plus court\n----------------------"<<endl<<endl;
    cout<<"\t\t";

    for (i = 0;i < max_time;i ++){
        cout << i <<"\t|\t" ;
    }
    cout << endl;

    for (i = 0;i < nb_vertex;i ++){
        
        if (this->vertex[i] == 'a' || this->vertex[i] == 'z') {
            continue;
        }
        
        cout <<this->vertex[i];
        date_begin = this->dateBegin[i];
        date_end = this->cost[i] + date_begin;

        for (j = 0;j <= max_time;j ++){
            cout << "\t|\t";
            if (j >= date_begin && date_end > j)
                cout << "=";
            
        }
        cout << endl;
    }
}

void Graphe::GanttEnd(){
    int i = 0, date_begin = 0, date_end = 0, j = 0;
    int nb_vertex = (int)this->vertex.size();
    int max_time = *max_element(this->dateEnd.begin(), this->dateEnd.end());
    
    max_time += this->cost[distance(this->dateEnd.begin(), max_element(this->dateEnd.begin(), this->dateEnd.end()))];
    
    cout<<endl<<endl<<"Dates au plus tard\n----------------------"<<endl<<endl;
    cout<<"\t\t";

    for (i = 0;i < max_time;i ++){
        cout << i <<"\t|\t" ;
    }
    cout << endl;

    for (i = 0;i < nb_vertex;i ++){
        
        if (this->vertex[i] == 'a' || this->vertex[i] == 'z') {
            continue;
        }
        
        cout <<this->vertex[i];
        date_begin = this->dateEnd[i];
        date_end = this->cost[i] + date_begin;

        for (j = 0;j <= max_time;j ++){
            cout << "\t|\t";
            if (j >= date_begin && date_end > j)
                cout << "=";
        }
        cout << endl;
    }
}
