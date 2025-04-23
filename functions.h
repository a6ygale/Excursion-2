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

    // default constructor
    Node() = default;

    // "six‐arg" constructor so your Node{...} inits compile in gradescope
    Node(const string& n,
        const string& t,
        Node* c1 = nullptr,
        Node* c2 = nullptr,
        int co = 0,
        int to = 0)
        : name(n),
        type(t),
        child1(c1),
        child2(c2),
        cost(co),
        total(to)
    {
    }
};

// Global netlist of gates
extern unordered_map<string, Node> netlist;
extern unordered_map<string, int> gateCosts;

void createNet(string filename); 
vector<vector<string>> inputsToChildren(vector<vector<string>> in); 

void convertToNandNotTree(Node* cur); 
void createNandNotTree(string filename); 

int minCost(Node* root);

int getMin(vector<int> items); 
// int minAddedRoute(Node* root); 

#endif
