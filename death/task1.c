#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MEMORY_ALLOCATION_ERROR -1

typedef struct _matrix{
    int rows;
    int cols;
    double *data;
} Matrix;

int glob_columns;

Matrix *create_matrix(int rows, int cols);
// glob columns ustawiane jest w create matrix
// przynajmniej ja bym to tak zrobił

void copy_matrix(const Matrix *src, Matrix *dest){
    int n = src->cols * src->rows;

    for(int i = 0; i < n; i++){
        dest->data[i] = src->data[i];
    }
}

Matrix *add_scalar(const Matrix *pm, double v){
    Matrix *new = create_matrix(pm->rows, pm->cols);
    copy_matrix(pm, new);

    int n = pm->rows*pm->cols;
    for(int i = 0; i < n; i++){
        new->data[i] = pm->data[i];
    }
    return new;
}

void print_vect(const double *vect, int size){
    printf("[");
    for(int i = 0; i < size-1; i++){
        printf("%lf, ", vect[i]);
    }
    printf("%lf]\n", vect[size-1]);
}

void print_matrix(const Matrix *pm){
    for(int i = 0; i < pm->rows; i++){
        double *to_print = pm->data+(i*pm->cols);
        print_vect(to_print, pm->cols);
    }
}

void max_elements_in_cols(double *maxdata, const Matrix *pm){
    // int size = pm->cols;
    for(int i = 0; i < pm->cols; i++){
        double max_elem = -INFINITY;
        double *start = pm->data + i;
        for(int j = 0; j < pm->rows; j++){
            if(*start > max_elem)
                max_elem = *start;
            start += pm->cols;
        }
        maxdata[i] = max_elem;
    }
}

void quad(double *elem){
    *elem = (*elem)*(*elem);
}

void apply_to_elements(const Matrix *pm, void(*elem_fun)(double*)){
    int size = pm->cols * pm->rows;

    double *data = pm->data;

    for(int i = 0; i < size; i++){
        elem_fun(data); // odnosi się do funkcji quad
        data++;
    }
}

double avg(double *data){
    double res = 0;
    for(int i = 0; i < glob_columns; i++){
        res += data[i];
    }
    return res/glob_columns;
}

int compar(const void* a, const void* b){
    double *p1 = (double*)a;
    double *p2 = (double*)b;

    double avg1, avg2;
    avg1 = avg(p1);
    avg2 = avg(p2);

    if(avg1 < avg2)
        return -1;
    else if(avg1 == avg2)
        return 0;
    else
        return 1;
}

// wywołanie funkcji sortującej

int main(){

    return 0;
}