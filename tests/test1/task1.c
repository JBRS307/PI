#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _matrix{
    int rows;
    int cols;
    double **data;
} Matrix;

int create_matrix(Matrix *pmatrix, int rows, int cols){
    if(rows < 1 || cols < 1)
        return 0;
    
    pmatrix->rows = rows;
    pmatrix->cols = cols;

    pmatrix->data = (double**)malloc(rows*sizeof(double*));
    for(int i = 0; i < rows; i++){
        pmatrix->data[i] = (double*)malloc(cols*sizeof(double));
    }
    return 1;
}

int get(Matrix *pm, int row, int col, double *pvalue){
    if(row >= pm->rows || col >= pm->cols ||
       row < 0 || col < 0) // zakładam numerowanie od 0
        return 0;
    
    *pvalue = pm->data[row][col];
    return 1;
}

int set(Matrix *pm, int row, int col, double value){
    if(row >= pm->rows || col >= pm->cols ||
       row < 0 || col < 0)
        return 0;
    
    pm->data[row][col] = value;
}

void create_identity_matrix(Matrix *pm, int size){
    if(size < 1)
        return;
    
    pm->rows = size;
    pm->cols = size;

    pm->data = (double**)calloc(size, sizeof(double*));
    for(int i = 0; i < size; i++){
        pm->data[i] = (double*)calloc(size, sizeof(double));
        pm->data[i][i] = 1;
    }
}

double dot_prod_vect(const double *v1, const double *v2, int size){
    if(size < 1)
        return 0.0;

    double prod = 0.0;
    for(int i = 0; i < size; i++){
        prod += v1[i]*v2[i];
    }
    return prod;
}

void mul_vect(double *result, const Matrix *pm, const double *vect){
    int size = pm->cols;

    for(int i = 0; i < size; i++){
        result[i] = dot_prod_vect(pm->data[i], vect, size);
    }
}

double max_in_arr(double *arr, int size){
    double max = -INFINITY;
    for(int i = 0; i < size; i++){
        max = max > arr[i] ? max : arr[i];
    }
    return max;
}

void max_element_in_rows(double *maxdata, const Matrix *pm){
    int size = pm->rows;

    for(int i = 0; i < size; i++){
        maxdata[i] = max_in_arr(pm->data[i], pm->cols);
    }
}

void print_mat(Matrix *pm){
    for(int i = 0; i < pm->rows; i++){
        for(int j = 0; j < pm->cols; j++){
            printf("%.2lf ", pm->data[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

int main(){
    Matrix m;
    create_matrix(&m, 3, 3);
    set(&m, 0, 0, 10.0);
    set(&m, 0, 1, 6.0);
    set(&m, 0, 2, 7.0);
    set(&m, 1, 0, 3.0);
    set(&m, 1, 1, 9.0);
    set(&m, 1, 2, 1.0);
    set(&m, 2, 0, 5.0);
    set(&m, 2, 1, 5.0);
    set(&m, 2, 2, 5.0);

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            double value;
            get(&m, i, j, &value);
            printf("%.2lf ", value);
        }
        putchar('\n');
    }
    putchar('\n');

    double vect[] = {9.0, 5.0, 5.0};
    double res[3];
    mul_vect(res, &m, vect);
    for(int i = 0; i < 3; i++){
        printf("%.2lf ", res[i]);
    }
    putchar('\n');

    Matrix id;
    create_identity_matrix(&id, 5);
    print_mat(&id);

    return 0;
}