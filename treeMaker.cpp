#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
using namespace std;

#include "functions.h"

// Author: Max Banach
// 4/21/2025

/*

// Global netlist of gates
unordered_map<string, Node> netlist;
*/


void convertToNandNotTree(Node* cur) {
    if (!cur) return;

    // 1) recurse on children first
    if (cur->child1) convertToNandNotTree(cur->child1);
    if (cur->child2) convertToNandNotTree(cur->child2);

    // 2) AND  ?  NOT( NAND )
    if (cur->type == "AND") {
        Node* A = cur->child1;
        Node* B = cur->child2;

        // create the intermediate NAND2
        string nandName = cur->name + "_NAND";
        netlist[nandName] = Node{ nandName, "NAND2", A, B, gateCosts["NAND2"], 0 };

        // wrap it with a single NOT
        string notName = cur->name + "_NOT";
        netlist[notName] = Node{ notName, "NOT", &netlist[nandName], nullptr, gateCosts["NOT"], 0 };

        // splice that NOT in place of this node
        cur->type = "NOT";
        cur->child1 = &netlist[notName];
        cur->child2 = nullptr;
        cur->cost = gateCosts["NOT"];
    }
    // 3) OR  ?  NAND( NOT A, NOT B )
    else if (cur->type == "OR") {
        Node* A = cur->child1;
        Node* B = cur->child2;

        // NOT A
        string nA = cur->name + "_NOT1";
        netlist[nA] = Node{ nA, "NOT", A, nullptr, gateCosts["NOT"], 0 };

        // NOT B
        string nB = cur->name + "_NOT2";
        netlist[nB] = Node{ nB, "NOT", B, nullptr, gateCosts["NOT"], 0 };

        // NAND of the two NOTs
        string d = cur->name + "_NAND";
        netlist[d] = Node{ d, "NAND2", &netlist[nA], &netlist[nB], gateCosts["NAND2"], 0 };

        // splice that NAND into this node
        cur->type = "NAND2";
        cur->child1 = &netlist[nA];
        cur->child2 = &netlist[nB];
        cur->cost = gateCosts["NAND2"];
    }
    // 4) leaf nodes get their library cost
    else if (cur->type == "NOT") {
        cur->cost = gateCosts["NOT"];
    }
    else if (cur->type == "NAND2") {
        cur->cost = gateCosts["NAND2"];
    }
    // 5) everything else (INPUT, OUTPUT) stays cost=0
    else {
        cur->cost = 0;
    }
}

void createNandNotTree(string filename) {
    // 1) Read and parse exactly like createNet does
    ifstream file(filename);
    if (!file) {
        cerr << "Error: could not open \"" << filename << "\"\n";
        return;
    }

    vector<vector<string>> inputs;
    string line;
    bool seenOutput = false;

    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> toks;
        int i = 0, n = (int)line.size();

        // INPUT lines before `F OUTPUT`
        if (!seenOutput) {
            if (n > 2 && line[2] == 'O') {
                seenOutput = true;
            }
            else {
                // parse "a INPUT"
                for (int t = 0; t < 2; ++t) {
                    string tk;
                    while (i < n && line[i] != ' ') tk.push_back(line[i++]);
                    toks.push_back(tk);
                    if (i < n) ++i;
                }
                // record
                inputs.push_back(toks);
                Node v; v.name = toks[0]; v.type = toks[1];
                netlist[v.name] = v;
                continue;
            }
        }

        // gate lines: "t1 = AND b c"
        // 1) name
        string name;
        while (i < n && line[i] != ' ') name.push_back(line[i++]);
        if (i < n) ++i;
        // 2) skip '='
        if (i < n && line[i] == '=') ++i;
        if (i < n && line[i] == ' ') ++i;
        // 3) type
        string type;
        while (i < n && line[i] != ' ') type.push_back(line[i++]);
        if (i < n) ++i;
        toks.push_back(name);
        toks.push_back(type);
        // 4) inputs
        while (i < n) {
            string tk;
            while (i < n && line[i] != ' ') tk.push_back(line[i++]);
            toks.push_back(tk);
            if (i < n) ++i;
        }
        inputs.push_back(toks);

        Node g; g.name = name; g.type = type;
        netlist[name] = g;
    }
    file.close();

    // 2) Hook up children
    auto children = inputsToChildren(inputs);
    for (auto& kv : netlist) {
        Node& nd = kv.second;
        for (auto& r : children) {
            if (r[0] == nd.name) {
                if (!r[1].empty()) nd.child1 = &netlist[r[1]];
                if (!r[2].empty()) nd.child2 = &netlist[r[2]];
                break;
            }
        }
    }

    // 3) **only** convert the single root “F”
    Node* root = nullptr;
    for (auto& kv : netlist) {
        if (kv.second.type == "OUTPUT") {
            root = &kv.second;
            break;
        }
    }
    if (!root) {
        cerr << "Error: no OUTPUT node in netlist\n";
        return;
    }
    convertToNandNotTree(root);

    // 4) (optional) write total cost or dump the tree…
    int total = 0;
    for (auto& kv : netlist) total += kv.second.cost;
    ofstream out("output.txt");
    out << total << "\n";
    out.close();
}


// test
/*
int main(){
    createNandNotTree("input.txt");
    return 0;
}
*/
