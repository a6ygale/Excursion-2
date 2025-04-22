Excursion 2

Technology Mapping: uses netlist describing logic gates to find the 
minimum cost to reach output node. The cost described can be found in output.txt


Helpful functions: 

------------------------------------------------------------------------
void createNet(string filename); 
  take input.txt file and parses it to obtain information on node names, 
  their types, and inputs. 

  creates global unordered map "netlist" where you can call a node based
  on it's string name and it will return a pointer to the node that 
  contains its name, type, children (outputs), cost, and will later 
  contain cost to get to the end of network from current position


------------------------------------------------------------------------
vector<vector<string>> inputsToChildren(vector<vector<string>> in); 
  takes a n x j vector where the first index results to an individual gate
  the second index [0] contains the name of the gate and the items after 
  contain its cooresponding inputs (bottom up)
  
  the function then converts it to a n x j vector where the columns
  contain name and outputs of each gate to properly display the 
  network flow from top-down

------------------------------------------------------------------------
void convertToNandNotTree(Node* currentNode); 

------------------------------------------------------------------------
void createNandNotTree(string filename); 

------------------------------------------------------------------------
int minCost(Node* root);
  takes the starting node and recursively goes through all possible paths to output
  once calculated, pushes total value to a vector and calls getMin() to find lowest
  path value, returning it as an integer

------------------------------------------------------------------------

int getMin(vector<int> items)
  takes a vector of integers and returns the lowest value
