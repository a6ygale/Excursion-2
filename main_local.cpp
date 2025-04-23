#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <stack>

//comment out for local compile
#include "functions.h"

using namespace std; 

int main(){
    // 1 - create netlist from text file
    createNet("input.txt"); 
    
    // test output netlist for verification
    for (const auto& kv : netlist) {
        // kv.first  is the gate name
        // kv.second is the Node
        cout << "Gate: " << kv.first
            << "   type=" << kv.second.type;
        if (kv.second.child1)
            cout << "   child1=" << kv.second.child1->name;
        if (kv.second.child2)
            cout << "   child2=" << kv.second.child2->name;
        cout << "\n";
    }
    

    // 2 - find the output gate from when we read the net list
    // old approach looking for F, but not all output's are F (and not all F's are output)
    /*
    Node* root = &netlist.at("F");
    // now do your conversion + minCost
    convertToNandNotTree(root);
    */

    if (outputName.empty()) {
        cerr << "Error: no OUTPUT declared\n";
        return 1;
    }
    Node* root = &netlist.at(outputName);

    // 3 - convert to NAND-NOT tree
    convertToNandNotTree(root);

    // 4 - dump everything so we can eyeball it
    cout << "After NAND-NOT conversion:\n";
    for (auto& kv : netlist) {
        auto& n = kv.second;
        cout
            << "Gate: " << n.name
            << "  type=" << n.type
            << "  cost=" << n.cost;
        if (n.child1) cout << "  c1=" << n.child1->name;
        if (n.child2) cout << "  c2=" << n.child2->name;
        cout << "\n";
    }

    
    // 5 - total cost as a sanity check
    int total = 0;
    for (auto& kv : netlist) total += kv.second.cost;
    cout << "Total summed cost: " << total << "\n";

    // 6 - calculate minimum cost of converted NAND-NOT tree
    int minimal = minCost(root);
    cout << "Minimal total cost = " << minimal << "\n";

    // 7 - write to file
    ofstream fout("output.txt");
    fout << minimal << "\n";
    fout.close();

    return 0; 
}

/*


struct Node {
    string name;
    string type;
    Node* child1 = nullptr;
    Node* child2 = nullptr;
    int cost = 0; // Cost of the node based on the technology library
    int total = 0; 
};



//store all node names and cooresponding memory locations
unordered_map<string, Node> netlist; 



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
                        //cout<<in[gate][1]; 
                    }
                    if(prevChild ==0){
                        children[i][1] = in[gate][0]; //name of mentioned gate is child
                        prevChild = 1; 
                        //cout<<in[gate][0]; 
                    }
                }
                k++; 
            }
    
            gate++; 
        }

    }
     


    return children; 

}


///////////////////////////////////////////////////////////////////////////
// void createNet(string filename); 
//   take input.txt file and parses it to obtain information on node names, 
//   their types, and inputs. 

//   creates global unordered map "netlist" where you can call a node based
//   on it's string name and it will return a pointer to the node that 
//   contains its name, type, children (outputs), cost, and will later 
//   contain cost to get to the end of network from current position
///////////////////////////////////////////////////////////////////////


void createNet(string filename){

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
                cout<<"Gate: "<<children[i][0]<<"     Children: "<<children[i][1]<<" , "<<children[i][2]<<endl; 
            }
        }

    }
    

}

*/



