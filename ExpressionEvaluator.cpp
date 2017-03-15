//
//  ExpressionEvaluator.cpp
//  Avi Networks
//
//  Created by Vijetha Malla on 3/3/17.
//  Copyright © 2017 vyas. All rights reserved.
//

#include <iostream>
#include <stack>
#include <queue>
#include <sstream>
using namespace std;
#define isDigit(x) (x-'0' >=0 && x-'0' <= 9)
bool hasPrecedence(char op1, char op2) // true of op2 has higher precedence. op1 is expression op. op2 is ops stack top.
{
    if (op2 == '(' || op2 == ')')
        return false;
    if ((op1 == '*' || op1 == '/') && (op2 == '+' || op2 == '-'))
        return false;
    else
        return true;
}
int applyOp(char op, int b, int a)
{
    switch (op)
    {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
    }
    return 0;
}
int ExpressionEvaluator(string exp) { //assuming valid express;
    stringstream SEXP(exp);
    int num=0;
    stack<int> S;
    stack<char> ops;
    for(int i = 0; i< exp.size(); i++) {
        if(isDigit(exp[i])) {
            while(isDigit(exp[i])) {  num = num*10 + (exp[i]-'0'); /*if((i+1)<exp.size() && isDigit(exp[i+1]))*/ i++; };
            S.push(num);
            num = 0;
        }
        if(exp[i] == ' ') continue;
        else if (exp[i] == '(') ops.push(exp[i]);
        
        // Closing brace encountered, solve entire brace
        else if (exp[i] == ')')
        {
            while (ops.top() != '(') { //process all ops withint the ')'
                int b = S.top(); S.pop();
                int a = S.top(); S.pop();
                int tmp = applyOp(ops.top(), b, a);
                ops.pop();
                S.push(tmp);
            }
            ops.pop(); //pop the matching '('
        }
        else if (exp[i] == '+' || exp[i] == '-' ||
                 exp[i] == '*' || exp[i] == '/')
        {
            // While top of 'ops' has same or greater precedence to current
            // token, which is an operator. Apply operator on top of 'ops'
            // to top two elements in values stack
            while (!ops.empty() && hasPrecedence(exp[i], ops.top())) {
                int b = S.top(); S.pop();
                int a = S.top(); S.pop();
                int tmp = applyOp(ops.top(), b, a);
                ops.pop();
                S.push(tmp);
            }
            ops.push(exp[i]); //pop the matching '('
        }
    }
    while (!ops.empty()) {
        int b = S.top(); S.pop();
        int a = S.top(); S.pop();
        int tmp = applyOp(ops.top(), b, a);
        ops.pop();
        S.push(tmp);

    }
    return S.top();
}

int main() {
    string exp = "6 + (3 * 2)";
    int R = ExpressionEvaluator(exp);
    cout << R << endl;
}
