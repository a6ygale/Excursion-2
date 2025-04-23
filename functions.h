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
    int    cost = 0;
    int    total = 0;

    // default ctor
    Node() = default;

    // six-argument ctor so Node{…} works
    Node(const string& _name,
        const string& _type,
        Node* _c1,
        Node* _c2,
        int            _cost,
        int            _total)
        : name(_name)
        , type(_type)
        , child1(_c1)
        , child2(_c2)
        , cost(_cost)
        , total(_total)
    {
    }
};
// Global netlist of gates
extern unordered_map<string, Node> netlist;
extern unordered_map<string, int> gateCosts;
extern string outputName;

void createNet(string filename); 
vector<vector<string>> inputsToChildren(vector<vector<string>> in); 

void convertToNandNotTree(Node* cur); 
void createNandNotTree(string filename); 

int minCost(Node* root);

int getMin(vector<int> items); 
// int minAddedRoute(Node* root); 

#endif
