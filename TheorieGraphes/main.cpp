//
//  main.cpp
//  TheorieGraphes
//

#include <iostream>
#include <fstream>
#include "graphe.hpp"

int main(int argc, const char * argv[]) {
    
    string path;
    
    cout<<"Entrez le nom du fichier sans le .txt :"<<endl;
    cin>>path;
    cout<<endl<<endl;

    Graphe graphe(path+".txt");

    return 0;
}


