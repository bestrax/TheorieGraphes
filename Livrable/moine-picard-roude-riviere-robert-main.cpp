//
//  moine-picard-roude-riviere-robert-main.cpp
//  TheorieGraphes
//  MOINE-PICARD - ROUDE - RIVIERE - ROBERT

#include <iostream>
#include <fstream>
#include "moine-picard-roude-riviere-robert-graphe.hpp"

int main(int argc, const char * argv[]) {
    
    string path;
    
    cout<<"Entrez le nom du fichier sans le .txt :"<<endl;
    cin>>path;
    cout<<endl<<endl;

    Graphe graphe(path+".txt");

    return 0;
}


