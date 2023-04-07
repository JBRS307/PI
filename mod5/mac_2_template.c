#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ulong unsigned long

#define SIZE 40

#define TEST 0    // 1 - dla testow,  0 - dla oceny automatycznej

typedef char cbool;
#define true 1
#define false 0

void read_vec(double x[], size_t n) {
  for(size_t i=0; i<n; ++i)  scanf("%lf",x++);
}

void print_vec(const double x[], size_t n) {
  for(size_t i=0; i<n; ++i) printf("%.4f ",x[i]);
  printf("\n");
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


// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy
// kwadratowej. Wersja rozszerzona: Wybor wiersza z maksymalna waroscia
// |elementu glownego| Przy wymianie wierszy nalezy zastosowac wetor permutacji
// indeksow wierszy. Jezeli maksymalna wartosc |elementu glownego| < eps, to
// wyznacznik = 0. Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze
// byc niedokonczona. Jezeli wyznacznik != 0, i b,x != NULL, to w wektorze x
// umieszcza rozwiazanie ukladu rownan Ax=b.

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

// Return true on swap, false otherwise
cbool find_max_row(double A[][SIZE], int column, int n, int perm[SIZE]) {
  int max_i = column;
  for (int i = column; i < n; i++) {
    if (fabs(A[perm[i]][column]) > fabs(A[perm[max_i]][column])) {
      max_i = i;
    }
  }
  swap(&perm[max_i], &perm[column]);
  return max_i != column;
}

double weighted_sum(double A[], double X[], int start, int n) {
  double s = 0;
  for (int i = start; i < n; i++) {
    s += A[i] * X[i];
  }
  return s;
}

double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps) {
  if (n >= SIZE)
    abort();
  int p[SIZE];
  for (int i = 0; i < n; i++) {
    p[i] = i;
  }
  double B[SIZE];
  if (b != NULL) {
    for (int i = 0; i < n; i++)
      B[i] = b[i];
  } else {
    for (int i = 0; i < n; i++)
      B[i] = 1;
  }
  double det = 1;
  for (int y = 0; y < n; y++) {
    if (find_max_row(A, y, n, p)) {
      det *= -1;
    }
    det *= A[p[y]][y];
    if (fabs(A[p[y]][y]) < eps)
      return 0.0; 
    for (int i = y + 1; i < n; i++) {
      double factor = A[p[i]][y] / A[p[y]][y];
      for (int x = y; x < n; x++) {
        A[p[i]][x] -= A[p[y]][x] * factor;
      }
      B[p[i]] -= B[p[y]] * factor;
      A[p[i]][y] = 0;
    }
  }

  if (b != NULL && x != NULL) {
    for (int i = n - 1; i >= 0; i--) {
      int row = p[i];
      x[i] = (B[p[i]] - weighted_sum(A[row], x, i + 1, n)) / A[row][i];
    }
  }

  return det;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps) {
  int p[SIZE];
  double right[SIZE][SIZE];
  for (int i = 0; i < n; i++) {
    p[i] = i;
    for (int j = 0; j < n; j++) {
      right[i][j] = 0;
    }
    right[i][i] = 1;
  }
  double det = 1;

  for (int y = 0; y < n; y++) {
    if (find_max_row(A, y, n, p)) {
      det *= -1;
    }
    det *= A[p[y]][y];
    if (fabs(A[p[y]][y]) < eps)
      return 0.0;
    for (int i = y + 1; i < n; i++) {
      double factor = A[p[i]][y] / A[p[y]][y];
      for (int x = y; x < n; x++) {
        A[p[i]][x] -= A[p[y]][x] * factor;
      }
      for (int x = 0; x < n; x++)
        right[p[i]][x] -= right[p[y]][x] * factor;
      A[p[i]][y] = 0;
    }
  }
  for (int y = n - 2; y >= 0; --y) {
    for (int x = y + 1; x < n; x++) {
      double factor = A[p[y]][x] / A[p[x]][x];
      for (int i = 0; i < n; i++) {
        A[p[y]][i] -= A[p[x]][i] * factor;
        right[p[y]][i] -= right[p[x]][i] * factor;
      }
    }
  }
  for (int y = 0; y < n; y++) {
    double first_v = A[p[y]][y];
    for (int x = 0; x < n; x++) {
      A[p[y]][x] /= first_v;
      right[p[y]][x] /= first_v;
    }
  }
  for (int y = 0; y < n; y++) {
    int ix = p[y];
    for (int x = 0; x < n; x++) {
      B[y][x] = right[ix][x];
    }
  }
  return det;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;
	int to_do;
	size_t m,n,p,q;
	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
	switch (to_do) {
    case 1:
      if(TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
      scanf("%lu", &n);
      if(TEST) printf("Wpisz macierz A (wierszami): ");
      read_mat(A,n,n);
      if(TEST) printf("Wpisz wektor b: ");
      read_vec(b,n);
      det = gauss(A, b, x, n, eps);
      printf("%.4f\n",det);
      if(det) print_vec(x,n);
      break;
    case 2:
      if(TEST) printf("Wpisz rozmiar macierzy n = ");
      scanf("%lu",&n);
      if(TEST) printf("Wpisz elementy macierzy (wierszami): ");
      read_mat(A,n,n);
      det = matrix_inv(A,B,n,eps);
      printf("%.4f\n",det);
      if(det) print_mat(B,n,n);
      break;
    default:
      printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
