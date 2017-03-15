//
//  StringCompress.cpp
//  Avi Networks
//
//  Created by Vijetha Malla on 3/3/17.
//  Copyright © 2017 vyas. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

string encode(string str) {
    string R;
    int i = 0,j=0;
    if(str.size()==0) return str;
    while(i<str.size()) {
        auto f = str[i];
        j = str.find_first_not_of(&f, i, str.size()-i);
        if(j == str.size()) return R+f;
        if(j-i>1) R += (j-i)+'0';
        R +=f;
        i = j;
    }
    return R;
}
string decode (string str) {
    string R;
    if(str.size()==0) return R;
    int n=0;
    for(int i = 0; i<str.size();i++) {
        if(str[i]-'0' <0 || str[i]-'0' >9 ) {
            if(n==0) {
                R+=str[i];
                continue;
            } else {
                while(n--) R+=str[i];
            }
        }
        else n = n*10 + (str[i]-'0');
    }
    return R;
}
int main() {
    string str = "aabbcaaddddebba";
    cout << encode(str) << endl;
    cout << decode(str) << endl;
    return 0;
}
