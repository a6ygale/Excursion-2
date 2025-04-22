#include <iostream>
#include <vector> 
#include <unordered_map>
#include <fstream>
#include <string>
#include <stack>
using namespace std; 

#include "functions.h"


///////////////////////////////////////////////////////////////////////////////////////////
//  vector<vector<string>> inputsToChildren(vector<vector<string>> in); 
//
//  takes a n x j vector where the first index results to an individual gate
//  the second index [0] contains the name of the gate and the items after 
//  contain its cooresponding inputs (bottom up)
//
//  the function then converts it to a n x j vector where the columns
//  contain name and outputs of each gate to properly display the 
//  network flow from top-down
///////////////////////////////////////////////////////////////////////////////////////////



vector<vector<string>> inputsToChildren(vector<vector<string>> in){
    
    string current; 
    int k=1; //starts after name of node 
    bool prevChild; //does the node already have assigned child1

    //store new vector of node name and children
    vector<vector<string>>children(in.size(),vector<string>(3,"")); 


    for(int i=0; i<in.size();i++){
        current = in[i][0]; //for each gate well go through and check 
                            //if mentioned as input
        children[i][0]=current;
        
        prevChild = 0; //new gate, no previous children 
        int gate = 0; //indexing for each gate

        while(gate<in.size()){
            
            int k=1; //starts after name of node

            while(in[gate].size() > 1 && k < in[gate].size()){
                
                //if current gate is mentioned as an input, assign that gate as a child
                if (in[gate][k]==current){
                     
                    if(prevChild ==1){
                        children[i][2] = in[gate][0]; //name of mentioned gate is child
                        cout<<in[gate][1]; 
                    }
                    if(prevChild ==0){
                        children[i][1] = in[gate][0]; //name of mentioned gate is child
                        prevChild = 1; 
                        cout<<in[gate][0]; 
                    }
                }
                k++; 
            }
    
            gate++; 
        }

    }
     


    return children; 

}