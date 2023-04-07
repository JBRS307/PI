#include <stdio.h>

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int gcf(int a, int b){
    if(a < b){
        swap(&a, &b);
    }

    int temp = -1;
    while(temp != 0){
        temp = a%b;
        a = b;
        b = temp;
    }
    return a;
}

int main(){
    int a, b;
    scanf("%d%d", &a, &b);

    printf("%d\n", gcf(a, b));

    return 0;
}