#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 0
// #define DBG 0    // 1 - dla testow,  0 - dla oceny automatycznej

// 5.1.1

#define STRLEN_MAX  SIZE
#define KW_NO   32    // liczba zapisanych lancuchow (slow kluczowych)
#define ulong unsigned long

// n lancuchów wskazywanych w tablicy wskaznikow ptab kopiuje do tablicy tablic t2D   
void n_str_copy(char t2D[][STRLEN_MAX], char *ptab[], size_t n){
    for(ulong i = 0; i < n; i++){
        for(int j = 0; j < STRLEN_MAX; j++){
            t2D[i][j] = ptab[i][j];
            // if(ptab[i][j] == '\0') break;
        }
    }
}

int compare(const void *p1, const void *p2){
    // int len_a, len_b;
    const char *a = *(char**)p1;
    const char *b = *(char**)p2;
    return strcmp(a, b);
}

// Funkcja wypisuje w osobnych liniach n łańcuchów wskazywanych przez elementy tablicy ptab.
void print_ptab(char *ptab[], size_t n){
    for(ulong i = 0; i < n; i++){
        printf("%s\n", ptab[i]);
    }
}

// sortuje alfabetycznie n lancuchow wskazywanych w tablicy wskaznikow t  
void ptab_sort(char *ptab[], size_t n){ //sortuje przez wstawianie
    qsort(ptab, n, sizeof(ptab[0]), compare);
}

int compare_t2D(const void *p1, const void *p2, const char t2D[][STRLEN_MAX]){
    int *a = (int*)p1;
    int *b = (int*)p2;
    return -strcmp(t2D[*a], t2D[*b]);
}

// Porzadek odwrotny do alfabetycznego lancuchow zapisanych w tablicy t2D zapisuje w tablicy indices
void t2D_sort(const char t2D[][STRLEN_MAX], size_t indices[], size_t n){
    for(int i = 0; i < n; i++) indices[i] = i;
    qsort_r(indices, n, sizeof(indices[0]), compare_t2D, t2D);
}
// W wierszach tablicy t2D sa zapisane lancuchy znakowe w dowolnej kolejnosci.
// Tablica indeksow wierszy indices wyznacza porzadek tych lancuchow.
// Funkcja wypisuje w osobnych liniach łańcuchy wskazane przez n poczatkowych elementów tablicy indices.
void print_t2D_ind(const char (*ptr)[STRLEN_MAX], const size_t *pindices, size_t n) {
//void print_t2D_ind(const char ptr[][STRLEN_MAX], const size_t indices[], size_t n) {

    for(ulong i = 0; i < n; i++){
        printf("%s\n", ptr[pindices[i]]);
    }
}

// 5.1.2
// A mxp, B pxn

void mat_product(const double A[][SIZE], const double B[][SIZE], double AB[][SIZE], size_t m, size_t p, size_t n){
    //m - liczba wierszy A
    //p - liczba kolumn A i wierszy B
    //n - liczba kolumn B
    double elem = 0;
    for(ulong i = 0; i < m; i++){
        for(ulong j = 0; j < n; j++){
            for(ulong k = 0; k < p; k++){
                elem += A[i][k]*B[k][j];
            }
            AB[i][j] = elem;
            elem = 0;
        }
    }

}


// 5.1.3 Triangulatyzacja macierzy i obliczanie wyznacznika - wersja uproszczona
// (bez zamiany wierszy).
// Jezeli element glowny a[i][i] = 0, to triangularyzacja nie jest dokonczona,
// a wyznacznik = NAN
// Zalozenie: funkcja gauss moze zmienic wartosci elementow tablicy A

double gauss_simplified(double A[][SIZE], size_t n){
    double det = 1;
    double factor;
    for(ulong i = 0; i < n; i++){
        det *= A[i][i];
        if(A[i][i] == 0.0) return NAN;
        for(ulong j = i+1; j < n; j++){
            factor = A[j][i]/A[i][i];
            for(ulong k = i; k < n; k++){
                A[j][k] -= A[i][k]*factor;
            }
            A[j][i] = 0.0; //na wszelki wypadek, ponieważ dokładność double może być problematyczna
        }
    }
    return det;
}

void read_mat(double A[][SIZE], size_t m, size_t n) {
  for(size_t i=0; i<m; ++i) {
    for(size_t j=0; j<n; ++j)  scanf("%lf",&A[i][j]);
  }
}

void print_mat(const double A[][SIZE], size_t m, size_t n) {
  for(size_t i=0; i<m; ++i) {
    for(size_t j=0; j<n; ++j)  printf("%.4f ",A[i][j]);
    printf("\n");
  }
}

int main(void) {
    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE];

	int to_do;
	size_t m,n,p,q;

	char *keywords_ptab[] = {"do", "struct", "typedef", "for", "union",
    "int", "void", "long", "register", "auto", "return",
    "double", "else", "sizeof", "enum", "const", "continue",
    "default", "short", "extern", "static", "float",
    "goto", "switch", "if", "unsigned", "volatile",
    "while", "signed", "break", "char", "case"};
    char keywords_t2D[KW_NO][STRLEN_MAX];
    size_t indices[KW_NO];

	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
	switch (to_do) {
    case 1:
      n_str_copy(keywords_t2D, keywords_ptab, KW_NO);
      ptab_sort(keywords_ptab, KW_NO);
      t2D_sort(keywords_t2D, indices, KW_NO);
      if(TEST) printf("Wpisz liczbe lancuchow n ");
      scanf("%d", &n);   
      print_ptab(keywords_ptab, n);
      print_t2D_ind(keywords_t2D, indices, n);
      break;
    case 2:
      if(TEST) printf("Wpisz liczbe wierszy i liczbe kolumn mac.1: ");
      scanf("%d %d", &m, &n);
      if(TEST) printf("Wpisz mac.1 (wierszami): ");
      read_mat(A,m,n);
      if(TEST) printf("Wpisz liczbe wierszy i liczbe kolumn mac.2: ");
      scanf("%d %d", &p, &q);
      if(TEST) printf("Wpisz mac.2 (wierszami): ");
      read_mat(B,p,q);
      if(n == p) mat_product(A,B,C,m,n,q);
      print_mat(C,m,q);
      break;
    case 3:
      if(TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
      scanf("%d", &n);
      if(TEST) printf("Wpisz macierz A (wierszami): ");
      read_mat(A,n,n);
      printf("%.4f\n", gauss_simplified(A, n));
      print_mat(A,n,n);
      break;
    default:
      printf("NOTHING TO DO FOR %d\n", to_do);
  }
  return 0;
}

