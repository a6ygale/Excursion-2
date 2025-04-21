#include <iostream>
#include <vector> 
#include <unordered_map>
#include <fstream>
#include <string>
using namespace std; 

//using nodes to store value of 
struct Node{
    string name; 
    string type; 
    vector<string> inputs; 
};

unordered_map<string, Node> netlist; 

void createNet(string filename){

    string value; 

    string name; 
    string type; 

    bool start = 0; 

    ifstream file(filename);
    string line; //holds a line in txt file as string
    
    while (getline(file, line)) {
        vector<string> inputs = {}; 

        if (start ==1){

            int j=0; //iterates through characters in string
    
            for(int k=0; k<4;k++){//expecting 4 values per line
                value = ""; //clears string that we will use to collect individual values
                while(line[j]!=' '&& line[j]!=NULL){//parses sting and adds data to column vector
                    value = value +line[j];
                    j++;  
                }
                j+=1;//once we hit blank space skip over

                if (k==0){
                    name = value; 
                    cout<<"name: "<<value<<endl; 
                }

                if (k==2){
                    type = value; 
                    cout<<"type: "<<value<<endl; 
                }

                if (k==3){
                    j=j-2; 
                    while (line[j]!= NULL){
                        value = ""; 
                        while(line[j]!=' '&& line[j]!=NULL){
                            value = value +line[j]; 
                            j++; 
                        }
                        inputs.push_back(value); 
                        cout<<"Inputs: "<<value<<endl; 
                        j+=1; //skip whitespace
                    }

                }

            }

        }


        //goes through txt file to find start of net
        if (line[2] == 'O'){
            start = 1; 
        }



        //write the new node properties and adds to unordered map
        Node node; 
        node.name = name; 
        node.type = type; 
        node.inputs = inputs; 
        netlist[name] = node; 


    }
    file.close();

}

//  testing
// int main(){
//     createNet("input.txt"); 
    

//     return 0; 
// }