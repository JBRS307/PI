#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define N 100

#define TEST 0 // 1 - dla testowania
               // 0 - dla oceny automatycznej

void swap(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

// Losuje jedna liczbe calkowita z przedzialu [a,b] stosujac funkcje rand() i operator %
// Jezeli a>b to zwraca INT_MIN
// Jezeli b-a > RAND_MAX to zwraca INT_MAX
// Jezeli a=b to zwraca a (bez wywolywania funkcji rand)
// Else zwraca liczbe z przedzialu [a,b]
int rand_from_interval(int a, int b){
    if(a == b) return a;
    else if(a > b) return INT_MIN;
    else if(b-a > RAND_MAX) return INT_MAX;

    int interval = b-a+1;

    return rand()%interval + a;
}

// Losowa permutacja elementow zbioru liczb {0, 1, 2,..., n-1} (z rozkladem rownomiernym)
// wg algorytmu przedstawionego w opisie zadania
// jest zapisywana w tablicy tab.
// 0 < n <= 100, jezeli nie to elementy tablicy tab maja wartosci nieokreslone.
void rand_permutation(int n, int arr[]){
    for(int i = 0; i<n; i++){
        arr[i] = i;
    }
    for(int i = 0; i <= n-2; i++){
        int k = rand_from_interval(i, n-1);
        swap(&arr[i], &arr[k]);
    }

}

// Metoda babelkowa sortowania n elementow tablicy tab w porzadku od wartosci najmniejszej do najwiekszej.
// Zwraca numer iteracji petli zewnetrznej (liczony od 1), po ktorej tablica byla uporzadkowana,
// np. dla { 0 1 2 3 7 4 5 6 } -> 1,
//     dla { 1 2 3 7 4 5 6 0 } -> 7,
//     dla { 0 1 2 3 4 5 6 7 } -> 0.
int bubble_sort(int n, int arr[]){
    int counter = 0;
    int swaps = 0; //sprawdza czy są przesunięcia.
    int i, j;
    
    for(i = 0; i<n-1; i++, counter++){
        for(j = 0, swaps = 0; j<n-1-i; j++){
            if(arr[j] > arr[j+1]){
                swap(&arr[j], &arr[j+1]);
                swaps=1;
            }
        }
        if(swaps == 0) break;
    }
    return counter;
}

int main(void)
{
    int nr_testu, seed;
    int a, b, n, i;
    int tab[N];
    if (TEST)
        printf("Wpisz nr testu i seed = ");
    scanf("%d %d", &nr_testu, &seed);
    srand(seed); // ustawienie ziarna generatora (dla powterzalnosci wynikow)
    switch (nr_testu)
    {
    case 1:
        if (TEST)
            printf("a b = ");
        scanf("%d %d", &a, &b);
        for (int i = 0; i < 3; ++i)
            printf("%d ", rand_from_interval(a, b));
        break;
    case 2:
        if (TEST)
            printf("n = ");
        scanf("%d", &n);
        rand_permutation(n, tab);
        for (i = 0; i < n; ++i)
            printf("%d ", tab[i]);
        printf("\n");
        break;
    case 3:
        if (TEST)
            printf("n = ");
        scanf("%d", &n);
        rand_permutation(n, tab);
        printf("%d\n", bubble_sort(n, tab));
        break;
    default:
        if (TEST)
            printf("NOTHING TO DO!\n");
    }
    return 0;
}
