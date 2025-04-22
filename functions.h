#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <stack>

using namespace std; 
#ifndef FUNCTIONS_H
#define FUNCTIONS_H


// Node structure to store information about gates and their children

struct Node {
    string name;
    string type;
    Node* child1 = nullptr;
    Node* child2 = nullptr;
    int cost = 0; // Cost of the node based on the technology library
    int total = 0; 
};



void createNet(string filename); 
vector<vector<string>> inputsToChildren(vector<vector<string>> in); 

void convertToNandNotTree(Node* currentNode); 
void createNandNotTree(string filename); 

int getMin(vector<int> items); 
int minAddedRoute(Node* root); 

#endif
