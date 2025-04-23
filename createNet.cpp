#include <iostream>
#include <vector> 
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#include "functions.h"

//Author: Abygale Cochrane

///////////////////////////////////////////////////////////////////////////
// void createNet(string filename); 
//   take input.txt file and parses it to obtain information on node names, 
//   their types, and inputs. 

//   creates global unordered map "netlist" where you can call a node based
//   on it's string name and it will return a pointer to the node that 
//   contains its name, type, children (outputs), cost, and will later 
//   contain cost to get to the end of network from current position
///////////////////////////////////////////////////////////////////////

unordered_map<std::string, Node> netlist;


void createNet(string filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "Error: cannot open " << filename << "\n";
        return;
    }

    // temporary map from gate name → its input names
    unordered_map<string, vector<string>> childNames;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        // split line into tokens
        istringstream iss(line);
        vector<string> tok;
        string w;
        while (iss >> w) tok.push_back(w);

        if (tok.size() == 2) {
            // Declaration: "a INPUT" or "F OUTPUT"
            const string& name = tok[0];
            const string& type = tok[1];
            netlist[name] = Node{ name, type, nullptr, nullptr, 0, 0 };
            // no children yet—if it's OUTPUT we'll wire it below
        }
        else if (tok.size() == 3) {
            // Alias: "F = t5"
            const string& name = tok[0];
            const string& child = tok[2];
            // must already have netlist[name] from the OUTPUT declaration
            childNames[name] = { child };
        }
        else if (tok.size() >= 4) {
            // Gate: "g = AND x y" or "p = NOT a"
            const string& name = tok[0];
            const string& type = tok[2];
            vector<string> ins;
            for (size_t i = 3; i < tok.size(); ++i)
                ins.push_back(tok[i]);
            netlist[name] = Node{ name, type, nullptr, nullptr, 0, 0 };
            childNames[name] = move(ins);
        }
        // else: malformed line, ignore
    }
    in.close();

    // now wire up the Node* pointers
    for (auto& entry : childNames) {
        const string& gateName = entry.first;
        const vector<string>& inputs = entry.second;
        Node& gate = netlist[gateName];

        if (inputs.size() > 0)
            gate.child1 = &netlist.at(inputs[0]);
        if (inputs.size() > 1)
            gate.child2 = &netlist.at(inputs[1]);
    }
}


/*
void createNet(string filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "Error: could not open \"" << filename << "\"\n";
        return;
    }

    vector<vector<string>> rows;
    string line;
    bool gateMode = false;

    while (getline(in, line)) {
        if (line.empty())
            continue;

        // split on spaces
        istringstream iss(line);
        vector<string> tok;
        string w;
        while (iss >> w) {
            tok.push_back(w);
        }

        // before we hit the OUTPUT declaration, 
        // we only see lines like "a INPUT" or "F OUTPUT"
        if (!gateMode) {
            if (tok.size() == 2 && tok[1] == "INPUT") {
                // record an input node
                netlist[tok[0]] = Node{ tok[0], tok[1], nullptr, nullptr, 0, 0 };
            }
            else if (tok.size() == 2 && tok[1] == "OUTPUT") {
                // record the output node itself, then switch to gateMode
                netlist[tok[0]] = Node{ tok[0], tok[1], nullptr, nullptr, 0, 0 };
                gateMode = true;
            }
            continue;
        }

        // once in gateMode, lines look like "t1 = AND b c"
        if (tok.size() >= 3) {
            string name = tok[0];
            string type = tok[2];
            string in1 = tok.size() > 3 ? tok[3] : "";
            string in2 = tok.size() > 4 ? tok[4] : "";

            // stash the row for linking later
            rows.push_back({ name, type, in1, in2 });

            // create a blank node now, link children in the next pass
            netlist[name] = Node{ name, type, nullptr, nullptr, 0, 0 };
        }
    }

    in.close();

    // now link up child1/child2 pointers
    for (auto& r : rows) {
        // r = { gateName, type, childName1, childName2 }
        Node& n = netlist[r[0]];
        if (r.size() > 2 && !r[2].empty())
            n.child1 = &netlist[r[2]];
        if (r.size() > 3 && !r[3].empty())
            n.child2 = &netlist[r[3]];
    }
}
*/

//  testing
// int main(){
//     createNet("input.txt"); 
    
//     return 0; 
// }
