#include <stdio.h>

int main(){
    int n;
    scanf("%d", &n);

    unsigned long a = 0, b = 1;
    unsigned long temp;
    if(n == 1){
        printf("%lu", a);
    }
    else if(n >= 2){
        printf("%lu %lu ", a, b);
    }

    for(int i = 2; i < n; i++){
        temp = b;
        b += a;
        a = temp;
        printf("%lu ", b);
    }
    printf("\n");

    return 0;
}