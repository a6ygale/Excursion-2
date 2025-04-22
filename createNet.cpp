#include <iostream>
#include <vector> 
#include <unordered_map>
#include <fstream>
#include <string>
using namespace std;

#include "functions.h"

//Author: Abygale Cochrane


//using nodes to store value of 
struct Node{
    string name; 
    string type; 
    Node* child1 = nullptr; 
    Node* child2 = nullptr; 
};



void createGates(string filename){

    string value; //storage for parsed string elements
    string name; //the title on input / gate
    string type; //input output and or etc

    vector<vector<string>> inputs; //title of node and inputs to it
    

    bool start = 0; //indicates start of gates in file

    ifstream file(filename);
    string line; //holds a line in txt file as string
    
    while (getline(file, line)) {
        vector<string> Ninputs = {}; 
        int c = 0;//indicates index of input elements in string  


        //if it's a gate 
        if (start ==1){

            int j=0; //iterates through characters in string
    
            for(int k=0; k<4;k++){//expecting 4 values per line
                value = ""; //clears string that we will use to collect individual values
                while(line[j]!=' '&& line[j]!=NULL){//parses string
                    value = value +line[j];
                    j++;  
                }
                j+=1;//once we hit blank space skip over

                //name of node stored
                if (k==0){
                    name = value; 
                    Ninputs.push_back(name); 
                     
                }

                //type of node stored
                if (k==2){
                    type = value; 
                    c = j; //store index for end of type, beginning of inputs
                }

                //inputs of node stored
                if (k==3){
                     
                    while (line[c]!= NULL){
                        value = "";  
                        while(line[c]!=' '&& line[c]!=NULL){
                            value = value +line[c]; 
                            c++; 
                        }
                        
                        Ninputs.push_back(value); 
                         
                        c+=1; //skip whitespace
                    }

                }

            }


            inputs.push_back(Ninputs); //push into vector storing all nodes and cooresponding inputs

            //write the new node properties and adds to unordered map
            Node node; 
            node.name = name; 
            node.type = type; 
            netlist[name] = node; 
             
        }

         
        

        //goes through txt file to find start of net, skips over first mention of output
        if (line[2] == 'O'){
            start = 1; 
        }

        //handling direct inputs (a,b,c....)
        if (start == 0){
            int j=0; //iterates through characters in string
    
            for(int k=0; k<2;k++){//expecting 2 values per line
                value = ""; //clears string that we will use to collect individual values
                while(line[j]!=' '&& line[j]!=NULL){//parses string 
                    value = value +line[j];
                    j++;  
                }
                j+=1;//once we hit blank space skip over

                if (k==0){
                    name = value; 
                    Ninputs.push_back(name); 
                    inputs.push_back(Ninputs); //we only need name here
                }

                if (k==1){
                    type = value; 
                }
                }
                
            //write the new node properties and adds to unordered map
            Node node; 
            node.name = name; 
            node.type = type; 
            netlist[name] = node; 
            

        }


    }

    file.close();


    //check input array
    // for(int x = 0; x<inputs.size(); x++){

    //     for(int y = 0; y<inputs[x].size(); y++){
    //         cout<<"  "<<inputs[x][y]; 
    //     }
    //     cout<<endl; 
    // }


    //switches input nodes to cooresponding children for each node
    vector<vector<string>> children = inputsToChildren(inputs); 

   
    string current; //holds current position in netlist
    for(auto it = netlist.begin(); it != netlist.end(); it++){
        current = (it->first);  //first node name

        //if this node is mentioned as an input, make pointer to child
        for(int i = 0; i< inputs.size(); i++){
            if(children[i][0]==current){
                it->second.child1 = &netlist[children[i][1]]; 
                it->second.child2 = &netlist[children[i][2]]; 

                //check
                //cout<<"Gate: "<<children[i][0]<<"     Children: "<<children[i][1]<<" , "<<children[i][2]<<endl; 
            }
        }

    }
    

}



//  testing
// int main(){
//     createGates("input.txt"); 
    
//     return 0; 
// }
