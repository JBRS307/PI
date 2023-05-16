#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int *get(int cols, int row, int col, const int *A){
	int *adr = (int*)A;

	adr += cols*row+col;
	return adr;
}

void set(int cols, int row, int col, int *A, int value){
	int *adr = get(cols, row, col, A);

	*adr = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB){
	int rowsB = colsA;
	
	int ia = 0, ja = 0, ib = 0, jb = 0;
	for(int i = 0; i < rowsA*colsB; i++){
		int elem = 0;
		while(ja < colsA){
			elem += *get(colsA, ia, ja, A)*(*get(colsB, ib, jb, B));
			ja++;
			ib++;
		}
		set(colsB, ia, jb, AB, elem);
		ja = 0, ib = 0;
		ia++;
		if(ia >= rowsA){
			ia = 0;
			jb++;
		}
	}
}

void read_mat(int rows, int cols, int *t){
	int elems = rows*cols;

	for(int i = 0; i < elems; i++){
		scanf("%d", t);
		t++;
	}
}

void print_mat(int rows, int cols, int *t){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			int elem = *get(cols, i, j, t);
			printf("%d ", elem);
		}
		printf("\n");
	}
}


int read_char_lines(char *arr[]){
}

void write_char_line(char *arr[], int n) {
}

void delete_lines(char *array[]) {
}

int read_int_lines_cont(int *ptr[]){
	char line[BUF_SIZE];
	int ptr_ind = 0;
	size_t line_leng = 0;
	char *p, *e;
	long v;
	int num;

	while(fgets(line, BUF_SIZE, stdin)){
		line_leng = 0;
		p = line;
		for(p = line; ; p = e){
			v = strtol(p, &e, 10);
			if(p == e)
				break;
			
			num = (int)v;
			if(line_leng == 0){
				line_leng++;
				ptr[ptr_ind] = (int*)malloc(line_leng*sizeof(int));
			}else{
				line_leng++;
				ptr[ptr_ind] = (int*)realloc(ptr[ptr_ind], line_leng*sizeof(int));
			}
			ptr[ptr_ind][line_leng-1] = num;
		}
		ptr[ptr_ind] = (int*)realloc(ptr[ptr_ind], (line_leng+1)*sizeof(int));
		memmove(ptr[ptr_ind]+1, ptr[ptr_ind], line_leng*sizeof(int));
		ptr[ptr_ind][0] = line_leng;
		ptr_ind++;
	}
	return ptr_ind;
}

void write_int_line_cont(int *ptr[], int n){
	int *line = ptr[n];
	int len = line[0];
	for(int i = 1; i <= len; i++){
		printf("%d ", line[i]);
	}
	printf("\n");
}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
}

void write_int_line(line_type lines_array[], int n) {
}

void delete_int_lines(line_type array[], int line_count) {
}

int cmp (const void *a, const void *b) {
}

void sort_by_average(line_type lines_array[], int line_count) {
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
}

int cmp_triplets(const void *t1, const void *t2) {
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
}

void read_vector(int *v, int n) {
}

void write_vector(int *v, int n) {
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			int ptr_leng = read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			// for(int i = 1; i < ptr_leng; i++)
				// free(ptr_array[i]);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}


