#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef union {
	int int_data;
	char char_data;
	// ... other primitive types used
	void *ptr_data;
} data_union;

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void*);

typedef struct {
	size_t size;
	size_t no_elements;
	ht_element *ht;
	DataFp dump_data;
	CreateDataFp create_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data,
		     DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data){
				p_table->no_elements = 0;
                p_table->size = size;
                p_table->dump_data = dump_data;
                p_table->create_data = create_data;
                p_table->free_data = free_data;
                p_table->compare_data = compare_data;
                p_table->hash_function = hash_function;
                p_table->modify_data = modify_data;
                p_table->ht = (ht_element*)calloc(size, sizeof(ht_element)); // chcę mieć czysty obszar pamięci na głowy list
				if(!p_table->ht)
					exit(MEMORY_ALLOCATION_ERROR);
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, size_t n){
    ht_element *curr = p_table->ht[n].next;

	while(curr){
		p_table->dump_data(curr->data);
		curr = curr->next;
	}
	putchar('\n');
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete){
    if(free_data != NULL)
    	free_data(to_delete->data);
    free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table){
    for(size_t i = 0; i < p_table->size; i++){
        ht_element *curr = p_table->ht[i].next;
		ht_element *next;
		if(curr)
			next = curr->next;
		else
			next = NULL;

        while(curr){
            next = curr->next;
            free_element(p_table->free_data, curr);
            curr = next;
        }
    }
    free(p_table->ht);
}

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
	static const double c = 0.618033988; // (sqrt(5.) â€“ 1) / 2.;
	double tmp = k * c;
	return (size_t)floor((double)size * (tmp - floor(tmp)));
}

void insert_node(hash_table *p_table, ht_element *elem){
    p_table->no_elements++;
    size_t key = p_table->hash_function(elem->data, p_table->size);
    elem->next = p_table->ht[key].next;
    p_table->ht[key].next = elem;
}

void rehash(hash_table *p_table){
    size_t old_size = p_table->size;
    ht_element *old = p_table->ht;

    p_table->size <<= 1;
    p_table->no_elements = 0;
    p_table->ht = (ht_element*)calloc(p_table->size, sizeof(ht_element));
	if(!p_table->ht)
		exit(MEMORY_ALLOCATION_ERROR);

    for(size_t i = 0; i < old_size; i++){
        ht_element *curr = old[i].next;
        while(curr){
			ht_element *next = curr->next;
            insert_node(p_table, curr);
            curr = next;
        }
    }
    free(old);
}

// find element; return pointer to previous`
ht_element *find_previous(hash_table *p_table, data_union data){
	size_t key = p_table->hash_function(data, p_table->size);

	ht_element *curr = p_table->ht[key].next;
	if(!p_table->compare_data(data, curr->data))
		return &p_table->ht[key];
	
	ht_element *prev = curr;
	curr = curr->next;
	while(curr){
		if(!p_table->compare_data(data, curr->data))
			return prev;
		prev = curr;
		curr = curr->next;
	}
	return NULL;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data){
	size_t key = p_table->hash_function(*data, p_table->size);

	ht_element *curr = p_table->ht[key].next;
	while(curr){
		if(!p_table->compare_data(curr->data, *data))
			return curr;
		curr = curr->next;
	}
	return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data){
	ht_element *tester = get_element(p_table, data);
	if(tester){
		if(p_table->modify_data)
			p_table->modify_data(&tester->data);
		if(p_table->free_data)
			p_table->free_data(*data);
		return;
	}

	ht_element *new = (ht_element*)malloc(sizeof(ht_element));
	if(!new)
		exit(MEMORY_ALLOCATION_ERROR);
	new->next = NULL;
	new->data = *data;
	insert_node(p_table, new);
	if((double)p_table->no_elements/(double)p_table->size > MAX_RATE)
		rehash(p_table);
}

// remove element
void remove_element(hash_table *p_table, data_union data){
	ht_element *at = get_element(p_table, &data);
	if(!at)
		return;
	
	ht_element *prev = find_previous(p_table, data);

	prev->next = at->next;
	free_element(p_table->free_data, at);
	(p_table->no_elements)--;
}

// type-specific definitions

// int element

size_t hash_int(data_union data, size_t size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data){
	printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b){
	return a.int_data - b.int_data;
}

data_union create_int(void* value){
	int val;
	scanf("%d", &val);
	data_union new;
	new.int_data = val;
	if(value != NULL)
		value = &new;
	return new;
}

// char element

size_t hash_char(data_union data, size_t size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data){
	printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b){
	int p1 = (int)a.char_data;
	int p2 = (int)b.char_data;

	return p1-p2;
}

data_union create_char(void* value){
	char val;
	scanf(" %c", &val);
	data_union new;
	new.char_data = val;
	if(value != NULL)
		value = &new;
	return new;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data){
	DataWord *w = data.ptr_data;
	printf("%s ", w->word);
	printf("%d\n", w->counter);
}

void free_word(data_union data){
	DataWord *w = data.ptr_data;
	free(w->word);
	free(w);
}

int cmp_word(data_union a, data_union b){
	DataWord *s1 = (DataWord*)a.ptr_data;
	DataWord *s2 = (DataWord*)b.ptr_data;

	return strcmp(s1->word, s2->word);
}

size_t hash_word(data_union data, size_t size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

void modify_word(data_union *data){
	DataWord *w = data->ptr_data;

	w->counter++;
}

void set_lowercase(char *str, size_t len){
	for(size_t i = 0; i < len; i++)
		str[i] = tolower(str[i]);
}

data_union create_data_word(void *value){
	char *str = (char*)value;

	DataWord *p = (DataWord*)malloc(sizeof(DataWord));
	p->counter = 1;
	size_t len = strlen(str);
	p->word = (char*)malloc(len);
	strcpy(p->word, str);
	set_lowercase(p->word, len);
	data_union new;
	new.ptr_data = p;
	return new;
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream){
	char buff[BUFFER_SIZE];
	const char delims[] = " \r\t\n.,?!-:;";
	char *p = NULL;
	size_t len;
	while(fgets(buff, BUFFER_SIZE, stream)){
		p = strtok(buff, delims);
		while(p){
			data_union data = create_data_word(p);
			insert_element(p_table, &data);
			p = strtok(NULL, delims);
		}
	}
}

// test primitive type list
void test_ht(hash_table *p_table, int n) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
//		p_table->create_data(&data);
		data = p_table->create_data(NULL); // should give the same result as the line above
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	size_t index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;
	FILE *stream;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 2: // test char hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 3: // read words from text, insert into hash table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%zu\n", table.size);
			data = table.create_data(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	free_table(&table);

	return 0;
}


