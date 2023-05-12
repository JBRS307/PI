#include <stdio.h>
#include <stdlib.h>

#define N 10

int main(){
    FILE *stream;
    stream = fopen("foods0.txt", "r");
    if(stream == NULL)
        return 0;

    for(int i = 0; i < N; i++){
        char name[30];
        float price;
        int amount;
        int d, m, y;
        fscanf(stream, "%s %f %d %d %d %d", name, &price, &amount, &d, &m, &y);
        printf("%s %f %d %d %d %d\n", name, price, amount, d, m, y);
    }

    fclose(stream);

    return 0;    
}