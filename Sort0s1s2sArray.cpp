//
//  ExpressionEvaluator.cpp
//  Avi Networks
//
//  Created by Vijetha Malla on 3/3/17.
//  Copyright © 2017 vyas. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

void
sort0s1s2s(vector<int> &V) {
    vector<int> colors(3,0);
    for(auto c:V)
        colors[c]++;
    
    for(int v = V.size()-1; v >= 0; v--) {
        while(colors.back() == 0) colors.pop_back();
        V[v] = colors.size()-1; colors.back()--;
    }
}
int main() {
//    vector<int> V = {0,1,2,1,2,0,0,1,2,1,1,0,2};
    vector<int> V = {1,1,1,0,0,0};
    sort0s1s2s(V);
    for(auto v:V)
        cout << v <<",";
    cout << endl;
}
