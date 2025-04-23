#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
using namespace std;

#include "functions.h"

//Author: Abygale Cochrane

///////////////////////////////////////////////////////////////////////////////////////////
//  int minCost(Node* root);
//  
//  takes the starting node and recursively goes through all possible paths to output
//  once calculated, pushes total value to a vector and calls getMin() to find lowest
//  path value, returning it as an integer
//
///////////////////////////////////////////////////////////////////////////////////////////

// Technology library cost for different gates - taken from pdf
unordered_map<string, int> gateCosts = {
    {"NOT", 2},
    {"NAND2", 3},
    {"AND2", 4},
    {"NOR2", 6},
    {"OR2", 4},
    {"AOI21", 7},
    {"AOI22", 7}
};


int minCost(Node* n) {
    if (!n) return 0;
    if (n->type == "INPUT") return 0;
    if (n->type == "OUTPUT")  // just score its one child
        return minCost(n->child1);

    // compute child subcosts
    int c1 = 0;
    if (n->child1) {
        c1 = minCost(n->child1);
    }

    int c2 = 0;
    if (n->child2) {
        c2 = minCost(n->child2);
    }

    vector<int> candidates;

    // 1 - Pure NAND/NOT
    if (n->type == "NOT") {
        // either a single NOT
        candidates.push_back(gateCosts.at("NOT") + c1);
        // or a 2-input NAND with both inputs = A
        candidates.push_back(gateCosts.at("NAND2") + c1 + c1);
    }
    else if (n->type == "NAND2") {
        candidates.push_back(gateCosts.at("NAND2") + c1 + c2);
    }

    // 2 - AND2 pattern = NOT w/ child that is NAND2(A,B)
    if (n->type == "NOT"
        && n->child1
        && n->child1->type == "NAND2"
        && n->child1->child1 && n->child1->child2)
    {
        int a = minCost(n->child1->child1);
        int b = minCost(n->child1->child2);
        candidates.push_back(gateCosts.at("AND2") + a + b);
    }

    // 3 - OR2 pattern = NAND2( NOT A, NOT B )
    if (n->type == "NAND2"
        && n->child1 && n->child1->type == "NOT"
        && n->child2 && n->child2->type == "NOT")
    {
        int a = minCost(n->child1->child1);
        int b = minCost(n->child2->child1);
        candidates.push_back(gateCosts.at("OR2") + a + b);
    }

    // 4 - NOR2 pattern = NOT w/ child that is NAND2( NOT A, NOT B )
    if (n->type == "NOT"
        && n->child1 && n->child1->type == "NAND2"
        && n->child1->child1 && n->child1->child1->type == "NOT"
        && n->child1->child2 && n->child1->child2->type == "NOT")
    {
        int a = minCost(n->child1->child1->child1);
        int b = minCost(n->child1->child2->child1);
        candidates.push_back(gateCosts.at("NOR2") + a + b);
    }

    // 5 - AOI21 pattern = NOT w/ child that is NAND2( NAND2(A,B), C )
    if (n->type == "NOT"
        && n->child1 && n->child1->type == "NAND2"
        && n->child1->child1 && n->child1->child1->type == "NAND2"
        && n->child1->child2)
    {
        Node* mid = n->child1;
        int p = minCost(mid->child1->child1);
        int q = minCost(mid->child1->child2);
        int r = minCost(mid->child2);
        candidates.push_back(gateCosts.at("AOI21") + p + q + r);
    }

    // 6 - AOI22 pattern = NOT w/ child that is NAND2( NAND2(A,B), NAND2(C,D) )
    if (n->type == "NOT"
        && n->child1 && n->child1->type == "NAND2"
        && n->child1->child1 && n->child1->child1->type == "NAND2"
        && n->child1->child2 && n->child1->child2->type == "NAND2")
    {
        Node* m = n->child1;
        int p = minCost(m->child1->child1);
        int q = minCost(m->child1->child2);
        int r = minCost(m->child2->child1);
        int s = minCost(m->child2->child2);
        candidates.push_back(gateCosts.at("AOI22") + p + q + r + s);
    }

    // pick min from candidates
    return *min_element(candidates.begin(), candidates.end());
}