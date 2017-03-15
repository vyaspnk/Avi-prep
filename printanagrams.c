//
//  printpermutations.c
//
//  Created by Vyas on 3/9/17.
//  Copyright © 2017 vyas. All rights reserved.
//

#include <stdio.h>
#include <string.h>
void swap(int *arr, int i, int j) {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}
void print_permu( int *arr, int i , int size) {
    
    if ( i == size-1 ) {
        for ( int j=0; j < size ; j++ ) printf("%d,", arr[j] );
        printf("\n");
    }
    else {
        for ( int j = i; j < size ; j++ ) {
            swap(arr,i,j);
            print_permu( arr, i+1, size );
            swap(arr,i,j);
        }
    }
}
int main(){
    int arr[] = { 1, 2, 3 , 4 ,8, 34 };
    int size = sizeof(arr)/sizeof(arr[0]);
    print_permu(arr, 0, size );
    return 0;
}
