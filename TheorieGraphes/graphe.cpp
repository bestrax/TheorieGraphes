//
//  graphe.cpp
//  TheorieGraphes
//

#include "graphe.hpp"
#include <fstream>

//Constructeur par défaut
Graphe::Graphe() {
    this->vertex = vector< char >();
    this->cost = vector< int >();
    this->adjacent = vector< vector< bool > >();
    this->values = vector< vector< int > >();
    this->fmatrix = vector< vector< string > >();
}

//Constructeur avec chargement d'un fichier
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
    
    //Ouverture du fichier et initialisation des variables
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

    //On recupere le nombre de sommets
    file >> nbVertex;
    
    //On ajoute le sommet a de début
    if (nbVertex > 0) {
        this->addVertex('a', 0);
    }

    //On ajoute les sommets présent dans le fichier
    for (int i = 0; i< nbVertex; i++) {
        file >> name;
        file >> cost;
        if ( this->addVertex(name, cost) == false) {
            cout<<"Mauvaise formation du fichier : multiple declaration du meme sommet"<<endl<<endl;
            return;
        }
    }
    
    //On ajoute le sommet z de fin
    if (nbVertex > 0) {
        this->addVertex('z', 0);
    }

    //On affiche la duree des taches
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

    //On lit chaque ligne du fichier pour trouver les contraintes
    while (!file.eof()) {
        file >> line;

        if (line.size() >= 2) {
            origin = line[0];
            for (int i = 1; i<line.size(); i++) {
                //Si le sommet n'a pas de contrainte on s'arrete
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

    //On verifie si le sommet n'existe pas deja
    if (find(vertex.begin(), vertex.end(), name) != vertex.end())
        return false;

    //Sinon on ajoute le sommet et son cout
    this->vertex.push_back(name);
    this->cost.push_back(cost);

    return true;
}

bool Graphe::addArc(char origin, char destination) {

    //Si la matrice d'adjacence n'est pas initialise on l'initialise
    if (this->adjacent.size() == 0) {
        for(int i = 0; i < this->vertex.size(); i++) {
            this->adjacent.push_back(vector< bool > (this->vertex.size(), false));
        }
    }

    //Si la matrice des valeurs n'est pas initialise on l'initialise
    if (this->values.size() == 0) {
        for(int i = 0; i < this->vertex.size(); i++) {
            this->values.push_back(vector< int > (this->vertex.size(), 0));
        }
    }

    //On cherche les positions de l'origine et de la destination de l'arc
    int posOrigin = (int)(find(this->vertex.begin(), this->vertex.end(), origin) - this->vertex.begin());
    int posDestination = (int)(find(this->vertex.begin(), this->vertex.end(), destination) - this->vertex.begin());

    //Si la position de l'origine ou de la destination est negative c'est que le sommet existe pas donc le fichier est
    //mal forme donc on sort
    if(posOrigin < 0 || posDestination < 0)
        return false;

    //On ajoute l'arc dans la matric d'adjacence
    this->adjacent[posOrigin][posDestination] = true;
    
    //On verifie la presence de circuit auquel cas on supprime l'arc de la matrice d'adjacence et on sort avec une erreur
    //Sinon on rajoute le cout dans la matrice des valeurs
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
    
    //On verifie qu'on ne se trouve pas sur le sommet auquel on a commence sinon on a un circuit et on peut sortir
    if (pos == check)
        return true;
    
    //Si on rentre seulement dans la fonction on initialise le parametre par defaut a notre position actuelle
    if (pos == -1) {
        pos = check;
    }
    
    //On stocke tous les successeurs de notre sommet en cours
    for (int i = 0; i < this->vertex.size(); i++) {
        if (this->adjacent[pos][i] == true) {
            current.push_back(i);
        }
    }
    
    //On fait un appel recursif sur tous les successeurs
    for (int i = 0; i < current.size(); i++) {
        if (hasCircuit(check, current[i]) == true) {
            return true;
        }
    }
    
    //Si on arrive la c'est que pour ce sommet il n'y a pas de circuit dans les sucesseurs
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
    
    //On affiche les arcs associés aux contraintes
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
    
    //Rechercher des entrées en iterant sur tous les sommets
    for (int i = 0; i < this->vertex.size(); i++) {
        //Si c'est l'entrée ou la sortie on passe
        if (this->vertex[i] == 'a' || this->vertex[i] == 'z') {
            continue;
        }
        entry = true;
        
        //On parcourt toutes les contraintes pouvant mener à ce sommet
        for(int j = 0; j < this->vertex.size(); j++) {
            if (this->adjacent[j][i] == true) {
                entry = false;
            }
        }
        
        //Si on n'a pas trouvé de contrainte alors on peut ajouter un arc
        if (entry) {
            this->addArc('a', this->vertex[i]);
        }
    }
    
    //Recherche des sorties en iterant sur tous les sommets
    for (int i = 0; i < this->vertex.size(); i++) {
        //Si c'est l'entrée ou la sortie on passe
        if (this->vertex[i] == 'a' || this->vertex[i] == 'z') {
            continue;
        }
        
        exit = true;
        
        //On parcourt tous les successeurs de ce sommet
        for(int j = 0; j < this->vertex.size(); j++) {
            if (this->adjacent[i][j] == true) {
                exit = false;
            }
        }
        
        //Si on n'a pas trouve de successeurs alors c'est une sortie
        if (exit) {
            this->addArc(this->vertex[i], 'z');
        }
    }
    
}

void Graphe :: displayAdj ()
{
    int const numberVertex = (int)this->vertex.size();
    cout<<"Matrice d'adjacence\n----------------------"<<endl<<endl<<"\t\t";
    
    //On affiche les noms des sommets sur l'axe horizontal
    for (int i = 0; i < numberVertex; i++) {
        cout<< this->vertex[i]<< "\t|\t";
    }

    cout<<endl;

    //On affiche les noms des sommets sur l'axe vertical et les valeurs sur les lignes suivantes
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
    
    //On affiche les noms des sommets sur l'axe horizontal
    for (int i = 0; i < numberVertex; i++) {
        cout<< this->vertex[i]<< "\t|\t";
    }

    cout<<endl;

    //On affiche les noms des sommets sur l'axe vertical et les valeurs sur les lignes suivantes
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

    // On cree la table de la matrice
    for (i = 0; i < numberVertex; i++) {
        this->fmatrix.push_back(vector< string > (numberVertex));
    }

    // On met les donnees dans la table
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
    
    //On affiche les noms des sommets sur l'axe horizontal
    for (int i = 0; i < numberVertex; i++) {
        cout<< this->vertex[i]<< "\t|\t";
    }

    cout<<endl;

    //On affiche les noms des sommets sur l'axe vertical et les valeurs sur les lignes suivantes
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
    int calculated = 0;
    //On recupere le rang max et le nombre de vecteur
    const int max = *max_element(this->rank.begin(), this->rank.end());
    const int numberVertex = (int)this->vertex.size();
    int min = max, k= -1, costTask;

    //On initialise notre tableau
    this->dateBegin = vector< int >(numberVertex);
    
    cout<<endl<<"----------------------------------------------------\n Calcul du calendrier au plus tôt\n----------------------------------------------------"<<endl<<endl;

    //Tant que l'on a pas traite tous les sommets
    while (calculated < numberVertex) {

        min = max;
        //On cherche les elements du rang minimum que l'on a pas encore traite
        for (int i = 0; i < this->rank.size(); i++) {
            if (this->rank[i] > k && this->rank[i] < min) {

                current.clear();
                min = this->rank[i];
                current.push_back(i);

            } else if (this->rank[i] == min) {

                current.push_back(i);

            }
        }

        //On met a jour le numero des rangs minimums a traiter par la suite
        if (current.size() > 0) {
            k = this->rank[current[0]];
        }
        
        cout<<"Calcul pour le rang "<<patch::to_string(k)<<" comprenant les sommets suivants :"<<endl;
        
        //On regarde sur la colonne de chaque item du rang  la date de debut des predecesseurs et on choisi celui avec la somme
        //date de debut + cout la plus courte pour notre calcul de date de debut
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

        calculated += current.size();
        current.clear();
    }
}

void Graphe::computeDateEnd() {
    
    //On verifie qu'on a deja effecuter le calcul des dates au plus tot
    if (dateBegin.size() == 0) {
        printf("Merci d effectuer le calcul de date au plus cours au paravant de la date au plus tard");
        return;
    }
    
    vector< int > current;
    int calculated = 0;
    //On recupere le rang minimum et le maximum
    const int min = *min_element(this->rank.begin(), this->rank.end());
    int max = min, k= *max_element(this->rank.begin(), this->rank.end())+1;
    bool lastElement = true;
    
    this->dateEnd = vector< int >(this->vertex.size());
    
    cout<<endl<<"----------------------------------------------------\n Calcul du calendrier au plus tard\n----------------------------------------------------"<<endl<<endl;
    
    //Tant que l'on a pas traite tous les sommets
    while (calculated < this->vertex.size()) {
        
        max = min;
        //On cherche les elements du rang minimum que l'on a pas encore traite
        for (int i = 0; i < this->rank.size(); i++) {
            if (this->rank[i] < k && this->rank[i] > max) {
                current.clear();
                max = this->rank[i];
                current.push_back(i);
                
            } else if (this->rank[i] == max) {
                current.push_back(i);
            }
        }
        
        //On met a jour le numero des rangs minimums a traiter par la suite
        if (current.size() > 0) {
            k = this->rank[current[0]];
        }
        
        //Si c'est le premier element qu'on traite alors c'est l'element de sortie z
        if (lastElement) {
            
            //Sa date au plus tard correspond a la valeur maximale de la date au plus tot
            const int timeEnd = *max_element(this->dateBegin.begin(), this->dateBegin.end());
            cout<<"Calcul pour le rang "<<patch::to_string(k)<<" comprenant les sommets suivants :"<<endl;
            
            //On met sa date au plus tard dans le tableau
            for (int i = 0; i < current.size(); i++) {
                this->dateEnd[current[i]] = timeEnd;
                cout<<"- "<<this->vertex[current[i]]<<" terminant a la date "<<patch::to_string(timeEnd)<<endl;
            }
            lastElement = false;
            
        } else {
            
            int minSuc = 0;
            bool firstSuc;
            
            cout<<"Calcul pour le rang "<<patch::to_string(k)<<" comprenant les sommets suivants :"<<endl;
            
            //On regarde sur la ligne de chaque element du rang pour recuperer la date au plus tard minimum de ses successeurs

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
                
                //Sa date au plus tard sera la date au plus tard de ses successeurs moins son cout
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
    vector< int > ignore;

    //On initialise le tableau de rang
    if (this->rank.size() <= this->vertex.size() ) {
        this->rank = vector< int >(this->vertex.size());
    }

    //Tant que l'on a pas donne a tous les sommets un rang
    while (ignore.size() < this->vertex.size() ) {
        //On cherche les racines - les sommets qu'on ignore car on les a deja traite
        roots = searchRoot(this->adjacent, ignore);
        //On marque le rang de chaque sommet et on le rajoute dans la liste a ignore
        for (int i = 0; i < roots.size(); i++) {
            this->rank[roots[i]] = k;
            ignore.push_back(roots[i]);
        }
        //On supprime les sommets qu'on vient de marque et on incremente notre numero de rang suivant
        roots.clear();
        k++;
    }
}

vector< int > Graphe::searchRoot(vector< vector< bool > > &adjacentMatrix, vector< int > &ignore) {

    vector< int > roots;

    //On parcourt chaque case de notre matrice d'adjacence
    for (int i = 0; i < adjacentMatrix.size(); i++) {
        bool isRoot = true;
        //Si c'est un sommet qu'on a deja traite alors on le laisse de cote
        if (find (ignore.begin(), ignore.end(), i) == ignore.end()) {
            for (int j = 0; j <adjacentMatrix[i].size(); j++) {
                //Si il a un successeur qui n'est pas a ignore alors il faut marquer le sommet comme pas une entree
                if (adjacentMatrix[j][i] == true && find (ignore.begin(), ignore.end(), j) == ignore.end())
                    isRoot = false;
            }

            //Si c'est une entree on l'ajoute au tableau des entrees
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

    //On affiche les numeros pour l'entete (dates)
    for (i = 0;i < max_time;i ++){
        cout << i <<"\t|\t" ;
    }
    cout << endl;
    
    //Pour chaque sommet
    for (i = 0;i < nb_vertex;i ++){
        
        //On ignore les entrees a et z qui ont ete ajoutes
        if (this->vertex[i] == 'a' || this->vertex[i] == 'z') {
            continue;
        }
        
        //On affiche le nom du sommet, on calcule sa date de debut et de fin
        cout <<this->vertex[i];
        date_begin = this->dateBegin[i];
        date_end = this->cost[i] + date_begin;

        //on affiche le reste de la ligne
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

    //On affiche les numeros pour l'entete (dates)
    for (i = 0;i < max_time;i ++){
        cout << i <<"\t|\t" ;
    }
    cout << endl;

    //Pour chaque sommet
    for (i = 0;i < nb_vertex;i ++){
        
        //On ignore les entrees a et z qui ont ete ajoutes
        if (this->vertex[i] == 'a' || this->vertex[i] == 'z') {
            continue;
        }
        
        //On affiche le nom du sommet, on calcule sa date de debut et de fin
        cout <<this->vertex[i];
        date_begin = this->dateEnd[i];
        date_end = this->cost[i] + date_begin;

        //on affiche le reste de la ligne
        for (j = 0;j <= max_time;j ++){
            cout << "\t|\t";
            if (j >= date_begin && date_end > j)
                cout << "=";
        }
        cout << endl;
    }
}
