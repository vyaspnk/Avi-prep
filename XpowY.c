//
//  XpowY.c
//
//  Created by Vyas on 3/9/17.
//  Copyright © 2017 vyas. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char * multiply(char* num1, char* num2) {
    
    char *R = (char *)malloc(strlen(num1)+strlen(num2)+1); //allocate new memory.
    memset(R,'0',strlen(num1)+strlen(num2));
    *(R+strlen(num1)+strlen(num2)) = '\0';
    
    
    for(int i = strlen(num1) - 1; i>=0; i--) {
        int carry = 0;
        for(int j = strlen(num2) - 1; j>=0; j--) {
            int tmp = (R[i+j+1]-'0') + ((num1[i]-'0') * (num2[j]-'0')) + carry;
            carry = tmp/10;
            R[i+j+1] = (tmp%10)+'0';
        }
        if(carry) {
            R[i] = ( R[i] - '0') + carry+'0';
        }
    }
    free(num1);//free previous allocation.
    int k = 0;
    if(*R == '0') {
        while(*(R+k)=='0')k++;
        memcpy(R,R+k,strlen(R)-k);
        R[strlen(R)-k]='\0';
    }
        return R;
}
int main() {
    char *A = "2";
    char *B = "100";
    if(strlen(A)==0 || strlen(B)==0 ) { printf("Invalid Input\n"); return 0;}
    else if(strlen(A)==1 && *A=='0') { printf("%s^%s = 0\n",A,B); return 0; }

    char *R = (char *)malloc(1+1);
    *R = '1';
    *(R+1) = '\0';

    int y = atoi(B);
    while(y--) {
        R = multiply(R, A);
    }
    printf("%s^%s = %s\n",A,B,R);
    free(R);
    return 0;
}
