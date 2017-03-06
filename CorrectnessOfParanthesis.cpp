//
//  CorrectnessOfParanthesis.cpp
//  Avi Networks
//
//  Created by Vijetha Malla on 3/3/17.
//  Copyright © 2017 vyas. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
using namespace std;
bool
isValidParanthesis(string &V) {
    stack<char> S;
    unordered_map<char, char> PMAP = { {'}','{'},{')','('},{']','['} };
    // }{ ][ )( are not valid paranthesis.
    for(int i = 0; i < V.size();i++) {
        if(PMAP.find(V[i]) == PMAP.end()) S.push(V[i]);
        else if(!S.empty() && S.top() == PMAP[V[i]] ) S.pop();
        else S.push(V[i]);
    }
    return S.size()?false:true;
}
int main() {
    
	string V = "[{(())[{()}]}]";
    cout << V << " is " << (isValidParanthesis(V)?"Valid":"Not Valid") << endl;
    return 0;
}

