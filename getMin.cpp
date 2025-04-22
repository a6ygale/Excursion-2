#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <stack>
using namespace std;

#include "functions.h"


///////////////////////////////////////////////////////////////////////////////////////////
//  int getMin(vector<int> items)
//
//  takes a vector of integers and returns the lowest value
//
///////////////////////////////////////////////////////////////////////////////////////////



int getMin(vector<int> items){
    int i = 0; 
    int j = 0; 
    while(items.size()>j){
        //finding minimum by comparing values throughout vector
        if(items[i]>items[j]){
            i=j; 
        }
        
        j++; 
    }
    return items[i]; //return index
}