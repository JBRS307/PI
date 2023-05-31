#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR  -1
#define FILE_OPEN_ERROR          -2
#define LIST_ERROR               -3
#define TEST 0               // 1 dla testowania, 0 dla automatycznej oceny

typedef char cbool;
#define true 1
#define false 0

typedef struct tagListElement {
    struct tagListElement *next;
    void                  *data;
} Node; // Zmiana nazwy na Node, bo poprzednia była długa i irytująca

typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*,const void*);

typedef struct tagList {
    Node *head;
    Node *tail;
    int         size;
    ConstDataFp dumpData;
    DataFp      freeData;
    CompareDataFp compareData;
    DataFp      modifyData;
} List;

////////////////////////////////////////////////

// Funkcje pomocnicze
void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	printf("malloc error\n");
	exit(MEMORY_ALLOCATION_ERROR);
}
char *safe_strdup(char *str) {
	char *ptr = strdup(str);
	if (ptr) return ptr;
	printf("safe_strdup error\n");
	exit(MEMORY_ALLOCATION_ERROR);
}

////////////////////////////////////////////////////////////////
// Funkcje uniwersalne (ogolnego przeznaczenia) obsługi listy

// Inicjuje listę
void init_List(List *list, ConstDataFp dump, DataFp free, CompareDataFp cmp, DataFp modify) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->dumpData = dump;
    list->freeData = free;
    list->compareData = cmp;
    list->modifyData = modify;
}

// Wypisuje dane elemntów listy
void dumpList(const List* list) {
    if(list->dumpData == NULL) return;
    Node *i;
    for(i=list->head; i!=0; i=i->next) list->dumpData(i->data);
}

// Zwalnia pamięć elementów listy i danych
void freeList(List *list) {
    Node *current = 0;
    current = list->head;
    while(current) {
        Node *todelete = current;
        current = current->next;
        if(list->freeData) list->freeData(todelete->data); // zwolnienie pamięci struktury data
        free(todelete);              // zwolnienie pamięci elementu
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Uniwersalne funkcje obsługi elementów listy

// Dodaje element na początku listy
void pushFront(List *list, void *data){
    Node *new = (Node*)malloc(sizeof(Node));
    if(new == NULL)
        exit(MEMORY_ALLOCATION_ERROR);
    
    new->data = data;
    new->next = list->head;
    list->head = new;
    if(list->size == 0)
        list->tail = list->head;
    (list->size)++;
}

// Dodaje element na końcu listy
void pushBack(List *list, void *data){
    Node *new = (Node*)malloc(sizeof(Node));
    if(new == NULL)
        exit(MEMORY_ALLOCATION_ERROR);

    new->data = data;
    new->next = NULL;
    if(list->size == 0){
        list->tail = new;
        list->head = list->tail;
    }else{
        (list->tail)->next = new;
        list->tail = new;
    }
    (list->size)++;
}

// Usuwa pierwszy element listy
void popFront(List *list){
    if(list->head){
        Node *to_delete = list->head;
        if(list->tail == to_delete)
            list->tail = NULL;
        list->head = (list->head)->next;
        if(list->freeData)
            list->freeData(to_delete->data);
        free(to_delete);
        (list->size)--;
    }
}

// Odwraca kolejność elementów listy
void reverse(List *list){
    if(list->size < 2)
        return;

    Node *curr, *next, *prev;

    curr = list->head;
    next = NULL;
    prev = NULL;
    list->tail = curr;

    while(curr->next != NULL){
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    list->head = curr;
    (list->head)->next = prev;
    (list->tail)->next = NULL;
}

// Funkcje pomocnicze dla list uporzadkowanych 
// (porządek wyznacza funkcja wskazana polem compareData)

// Zwraca element w posortowanej liście, a jeżeli nie ma, to element poprzedzający (nie ma, to NULL)
Node* findInsertionPoint(const List *list, Node *element, cbool *in_list){
    Node *insert_point = NULL;
    Node *curr;

    for(curr = list->head; curr != NULL; curr = curr->next){
        if(list->compareData(curr->data, element->data)<=0)
            insert_point = curr;
        else
            break;
    }
    if(insert_point != NULL && list->compareData(insert_point->data, element->data) == 0)
        *in_list = true;

    return insert_point;
}


// Drugi parametr przekazuje do funkcji adres przydzielonej dla "nowych" danych pamięci.
// Jeżeli takie dane są już w elemencie listy, to dane tego elementu są modyfikowane
// funkcją wskazaną w polu modifyData, a pamięć "nowych" danych jest zwalniana.
// Jeżeli w liście takiego elementu nie ma, to nowy element jest tworzony
// i dopisany do listy zachowując uporządkowanie listy.
void insertInOrder(List *list, void *a){
    cbool in_list = false;
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->data = a;
    new_node->next = NULL;

    Node *insert_after = findInsertionPoint((const List*)list, new_node, &in_list);

    if(in_list)
        list->modifyData(a);

    if(insert_after == NULL){
        new_node->next = list->head;
        list->head = new_node;
    }else{
        new_node->next = insert_after->next;
        insert_after->next = new_node;
    }
    (list->size)++;
}


/////////////////// Funkcje do zadania 11.1.2

// Dodaje element na końcu listy bez korzystania z pola tail
void pushBack_v0(List *list, void *data){
    Node *new = (Node*)malloc(sizeof(Node));
    new->data = data;
    new->next = NULL;
    if(new == NULL)
        exit(MEMORY_ALLOCATION_ERROR);
    
    if(!list->head){
        list->head = new;
        list->tail = new;
        return;
    }
    Node *curr = list->head;

    while(curr->next != NULL)
        curr = curr->next;
    
    curr->next = new;
    list->tail = new;
}


// Funkcje warstwy obsługi danych wskazywanych w polach struktury List


//////  Dla zadania 11.1.1 i 11.1.2
// 1. Dana jest liczbą typu int

typedef int DataInt;

// Domyślam się, że chodzi o wypisanie elementów, następnym razem prosiłbym o uwzględnienie tego
// w poleceniu.
void dump_int(const void *d){
    DataInt *a = (DataInt*)d;
    printf("%d ", *a);
}

void free_int(void *d){
    free(d);
}

int cmp_int(const void *a, const void *b){
    DataInt i1 = *(DataInt*)a;
    DataInt i2 = *(DataInt*)b;

    return i1-i2;
}

// Przydziela pamięć i zapisuje w niej daną o wartości v
void *create_data_int(int v){
    DataInt *n = (DataInt*)malloc(sizeof(DataInt));
    *n = v;
    return n;
}

//////  Dla zadania 11.1.3 i 11.1.4
// 2. Dana jest zapisana w  strukturze typu DataWord

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word (const void *d){
    DataWord *word = (DataWord*)d;
    printf("%s ", word->word);
}

void free_word(void *d){
    DataWord *w = (DataWord*)d;
    free(w->word);
    free(w);
}

int cmp_word_alphabet(const void *a, const void *b) {
}

int cmp_word_counter(const void *a, const void *b) {
}

void modify_word(void *a) {
}

// Wypisz dane elementów spełniających warunek równości sprawdzany funkcją 
// wskazywaną w polu compareData nagłówka listy
void dumpList_word_if(List *plist, int n) {
}

// Przydziela pamięć dla łańcucha string i struktury typu DataWord.
// Do przydzielonej pamięci wpisuje odpowiednie dane.
// Zwraca adres struktury.
void *create_data_word(char *string, int counter){
    DataWord *new = (DataWord*)malloc(sizeof(DataWord));
    if(new == NULL)
        exit(MEMORY_ALLOCATION_ERROR);
    new->word = strndup(string, strlen(string));
    new->counter = counter;
}

//////////////////////////////////////////////////

/////////////////// Funkcje pomocnicze dla zadan 11.1.3 i 11.1.4

// Wyrazy ze strumienia wejściowego zapisuje w elementach listy
// w kolejności określonej ostatnim parametrem funkcji (cmp)
// - adresem funkcji porównującej typu CompareDataFp. Adres zerowy oznacza, 
// że kolejność elementów listy jest zgodna z kolejnością 
void stream_to_list(List *list, FILE *stream, CompareDataFp cmp) {
    DataWord *data;
    char *p, buf[BUFFER_SIZE];
    char delimits[] = " \r\t\n.,?!-:;";
    if(cmp) list->compareData = cmp;
    while(fgets(buf,BUFFER_SIZE,stream)) {
        p = strtok(buf, delimits);
        while(p) {
            data = create_data_word(p, 1); 
            if(cmp) insertInOrder(list,data);
            else    pushBack(list, data);
            p = strtok(NULL,delimits);
        }
    }
}

void list_test(List *plist, int n) {
	char op[2];
	int v;
	Node element, *pelement;
	DataInt data;
	for (int i = 0; i < n; ++i) {
        if(TEST) printf("Wpisz kod polecenia ");
		scanf("%s", op);
		switch (op[0]) {
			case 'f': // dodaj na początku listy
                if(TEST) printf("Wpisz wartość ");
				scanf("%d", &v);
				pushFront(plist,create_data_int(v));
				break;
			case 'b': // dodaj na końcu listy
                if(TEST) printf("Wpisz wartość ");
				scanf("%d", &v);
				pushBack(plist,create_data_int(v));
				break;
			case 'd': // usuń pierwszy element
			    popFront(plist);
				break;
            case 'r': // odwróć kolejność
                reverse(plist);
                break;
			case 'i': // dodaj wg porządku rosnącego (int) lub alfabetycznego (char),
			          // jeżeli element już istnieje, to akcja zależna od typu danych
                if(TEST) printf("Wpisz wartość ");
				scanf("%d",&v);
				insertInOrder(plist,create_data_int(v));
                break;
			default:
				printf("No such operation: %s\n", op);
				break;
		}
	}
}

int main(void) {
    int to_do, n, size;
    char buf[257];
    clock_t t_start, t_end;
    double t_total;
    FILE *stream;
    List list;
    Node *head;

	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
	switch (to_do) {
case 1:
	    if(TEST) printf("Wpisz liczbę poleceń ");
        scanf("%d",&n);
        head = NULL;
        init_List(&list, dump_int, free_int, cmp_int, NULL);
//        list_test(&list, n, int (cmp)(const void*, const void*))
        list_test(&list, n);
        dumpList(&list);
        freeList(&list);
        break;
case 2:
        if(TEST) printf("Wpisz liczbę elementów ");
        scanf("%d",&size);
        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushBack_v0(&list,create_data_int(i));
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Back bez tail.  Czas = %g s.\n",size,t_total);
        freeList(&list);

        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushFront(&list,create_data_int(i));
        reverse(&list);
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Front + revers. Czas = %g s.\n",size,t_total);
        freeList(&list);

        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushBack(&list,create_data_int(i));
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Back z tail.    Czas = %g s.\n",size,t_total);
        freeList(&list);
        break;
case 3:      // wypisz wszystkie słowa wczytane do elementów listy
        init_List(&list,dump_word,free_word,NULL,NULL);
        if(TEST) {
            stream = stdin;
            printf("Wpisz linie tekstu\n");
            stream_to_list(&list, stream, NULL);
            rewind(stream);
        }
        else {
            stream = fopen("Tekst_1.txt","r");
            if(stream == NULL) exit(FILE_OPEN_ERROR);
            stream_to_list(&list, stream, NULL);
            fclose(stream);
        }
        dumpList(&list);
        freeList(&list);
        break;
case 4:      // wypisz w porządku alfabetycznym słowa o zadanej krotności
        if(TEST) printf("Wpisz krotność słowa ");
        scanf("%d",&n);
        init_List(&list,dump_word,free_word,NULL,modify_word);
        if(TEST) {
            stream = stdin;
            printf("Wpisz linie tekstu\n");
            stream_to_list(&list, stream, cmp_word_alphabet);
            rewind(stream);
        }
        else {
            stream = fopen("Tekst_C.txt","r");
            if(stream == NULL) exit(FILE_OPEN_ERROR);
            stream_to_list(&list, stream, cmp_word_alphabet);
            fclose(stream);
        }
        list.compareData = cmp_word_counter;
        dumpList_word_if(&list,n); printf("\n");
        freeList(&list);
        break;
default:
		printf("NOTHING TO DO FOR %d\n", to_do);
	}
    return 0;
}