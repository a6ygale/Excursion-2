#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
using namespace std;

//#include "functions.h

// Author: Max Banach
// 4/21/2025

// Node structure to store information about gates and their children
/*
struct Node {
    string name;
    string type;
    Node* child1 = nullptr;
    Node* child2 = nullptr;
    int cost = 0; // Cost of the node based on the technology library
    int total = 0; // combined cost to end from curren net position
};

// Global netlist of gates
unordered_map<string, Node> netlist;
*/

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



// Helper function to create a NAND-NOT tree from the original expression
void convertToNandNotTree(Node* currentNode) {
    if (!currentNode) {
        return;
    }

    // Base case: If the node is already a NOT, no conversion needed
    if (currentNode->type == "NOT") {
        currentNode->cost = gateCosts["NOT"];
        return;
    }

    // Recursive case: Convert children first (if they exist)
    if (currentNode->child1) {
        convertToNandNotTree(currentNode->child1);
    }
    if (currentNode->child2) {
        convertToNandNotTree(currentNode->child2);
    }

    // Now convert based on the gate type:
    if (currentNode->type == "AND") {
        // A AND B = NOT (A NAND B)
        string newNodeName = currentNode->name + "_NAND";
        Node newNandNode;
        newNandNode.name = newNodeName;
        newNandNode.type = "NAND2";
        newNandNode.child1 = currentNode->child1;
        newNandNode.child2 = currentNode->child2;
        netlist[newNodeName] = newNandNode;

        // The NOT gate (NOT (A NAND B))
        string notNodeName = currentNode->name + "_NOT";
        Node notNode;
        notNode.name = notNodeName;
        notNode.type = "NOT";
        notNode.child1 = &netlist[newNodeName];
        netlist[notNodeName] = notNode;

        currentNode->type = "NOT";
        currentNode->child1 = &netlist[notNodeName];
        currentNode->cost = gateCosts["NOT"] + gateCosts["NAND2"];
    }
    else if (currentNode->type == "OR") {
        // A OR B = (NOT A) NAND (NOT B)
        string newNot1Name = currentNode->name + "_NOT1";
        Node newNot1;
        newNot1.name = newNot1Name;
        newNot1.type = "NOT";
        newNot1.child1 = currentNode->child1;
        netlist[newNot1Name] = newNot1;

        string newNot2Name = currentNode->name + "_NOT2";
        Node newNot2;
        newNot2.name = newNot2Name;
        newNot2.type = "NOT";
        newNot2.child1 = currentNode->child2;
        netlist[newNot2Name] = newNot2;

        // NAND of the two NOTs
        string nandNodeName = currentNode->name + "_NAND";
        Node nandNode;
        nandNode.name = nandNodeName;
        nandNode.type = "NAND2";
        nandNode.child1 = &netlist[newNot1Name];
        nandNode.child2 = &netlist[newNot2Name];
        netlist[nandNodeName] = nandNode;

        currentNode->type = "NAND2";
        currentNode->child1 = &netlist[nandNodeName];
        currentNode->cost = gateCosts["NOT"] * 2 + gateCosts["NAND2"];
    }

    // Assign the correct cost for the current node
    if (currentNode->type == "NOT") {
        currentNode->cost = gateCosts["NOT"];
    }
    else if (currentNode->type == "NAND2") {
        currentNode->cost = gateCosts["NAND2"];
    }
    else if (currentNode->type == "AND2") {
        currentNode->cost = gateCosts["AND2"];
    }
    else if (currentNode->type == "OR2") {
        currentNode->cost = gateCosts["OR2"];
    }
}

void createNandNotTree(string filename) {
    ifstream file(filename);
    string line;
    vector<vector<string>> inputs;

    while (getline(file, line)) {
        vector<string> Ninputs;
        size_t pos = 0;
        string token;
        while ((pos = line.find(' ')) != string::npos) {
            token = line.substr(0, pos);
            Ninputs.push_back(token);
            line.erase(0, pos + 1);
        }
        Ninputs.push_back(line);
        inputs.push_back(Ninputs);

        string name = Ninputs[0];
        string type = Ninputs[2];

        Node node;
        node.name = name;
        node.type = type;
        netlist[name] = node;
    }

    // Assign children nodes
    for (auto& entry : netlist) {
        Node* node = &entry.second;
        for (auto& input : inputs) {
            if (input[0] == node->name) {
                node->child1 = &netlist[input[1]];
                node->child2 = &netlist[input[2]];
                break;
            }
        }
    }

    // Start converting from output to input
    for (auto& entry : netlist) {
        Node* node = &entry.second;
        convertToNandNotTree(node);
    }

    // Output the minimal cost
    int totalCost = 0;
    for (auto& entry : netlist) {
        totalCost += entry.second.cost;
    }

    ofstream outputFile("output.txt");
    outputFile << totalCost << endl;
    outputFile.close();
}

int main(){
    createNandNotTree("input.txt");
    return 0;
}
