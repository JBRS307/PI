#include <stdio.h>

float absolute(float n){
    return (n < 0 ? -n : n);
}

float root(float n, float eps){
    float a = n/2;
    float b = n/a;

    while(absolute(a-b) > eps){
        a = (a+b)/2;
        b = n/a;
    }
    return a;
}

int main(){
    float n, eps;
    scanf("%f%f", &n, &eps);

    printf("%f\n", root(n, eps));

    return 0;
}