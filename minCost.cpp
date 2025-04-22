#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <stack>
using namespace std;

#include "functions.h"

///////////////////////////////////////////////////////////////////////////////////////////
//  int minCost(Node* root);
//  
//  takes the starting node and recursively goes through all possible paths to output
//  once calculated, pushes total value to a vector and calls getMin() to find lowest
//  path value, returning it as an integer
//
///////////////////////////////////////////////////////////////////////////////////////////

int minCost(Node* root){


    vector<int> totals; 
     
    std::stack<Node*> s;
    s.push(root);
    root->total = root->cost; 

    while (!s.empty()) {
    // Pop node from stack to process
    Node* curr = s.top();
    s.pop();

    //tests current node information
    //cout<<curr->data<<endl<<curr->length<<endl<<endl;  


    // Push child2 first so that child1 is processed first
    // (child2 will only be processed once all subtrees in child1 are processed)
    if (curr->child1) {
        s.push(curr->child1);
        curr->child1->total = curr->child1->cost + curr->total; 
    }

    if (curr->child2) {
        s.push(curr->child2);
        curr->child2->total = curr->child2->cost + curr->total; 
    }

    //if no children add as path length (we hit output)
    if(!curr->child1 && !curr->child2){
        totals.push_back(curr->total); 
        
    }

    }

    //find the shortest path and return
    return getMin(totals); 
}