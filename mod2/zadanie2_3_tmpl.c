#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#define CBUFF_SIZE 52
#define COLORS 4

enum game_state{
    WAR = 0,
    A_WINS = 1,
    B_WINS = 2,
    OUT_OF_CONFLICTS = -1,
    OUT_OF_CARDS = -2,
};

//Funkcje gry
void split_cards(int full_deck[], int A[], int B[]);
int conflict(int A[], int B[], int out_a, int out_b);
void clear_arr(int arr[], int n);
int war(int A[], int B[], int out_a, int out_b, int len_a, int len_b, int *conflicts, int *turns);

//funkcje kolejki
double cbuff_push(int card, int cbuff[], int out, int *len);
int cbuff_pop(int cbuff[], int *out, int *len);
// int cbuff_state(void);
void cbuff_print(int cbuff[], int len, int out);

//funkcje losowania
void swap(int *a, int *b);
int rand_from_interval(int a, int b);
void rand_permutation(int n, int arr[]);

int main(){
    int seed, game, conflicts; //zmienne do zebrania od użytkownika
    int out_a = 0, out_b = 0; //następna wyłożona
    int len_a = CBUFF_SIZE/2, len_b = CBUFF_SIZE/2; //ilość kart
    int A[CBUFF_SIZE] = {0}; //gracz 0
    int B[CBUFF_SIZE] = {0}; //gracz 1
    int full_deck[CBUFF_SIZE];
    int battles;
    int turns;
    int end_cond = 0; //-1 - za mało konfliktów, -2 - koniec kart w czasie wojny, 1 - wygrał A, 2 - wygrał B

    scanf("%d%d%d", &seed, &game, &conflicts);
    int start = conflicts;
    srand(seed);
    split_cards(full_deck, A, B);

    int card_1, card_2;
    while(1){
        if(len_a == 0){
            end_cond = B_WINS;
            break;
        }
        else if(len_b == 0){
            end_cond = A_WINS;
            break;
        }
        else if(conflicts == 0){
            end_cond = OUT_OF_CONFLICTS;
            break;
        }
        switch(conflict(A, B, out_a, out_b)){
            case A_WINS: //wygrał A
                card_1 = cbuff_pop(A, &out_a, &len_a);
                card_2 = cbuff_pop(B, &out_b, &len_b);
                cbuff_push(card_1, A, out_a, &len_a);
                cbuff_push(card_2, A, out_a, &len_a);
            break;
            case B_WINS: //wygrał B
                card_1 = cbuff_pop(B, &out_b, &len_b);
                card_2 = cbuff_pop(A, &out_a, &len_a);
                cbuff_push(card_1, B, out_b, &len_b);
                cbuff_push(card_2, B, out_b, &len_b);
            break;
            case WAR:
                if(game == 1){ //Uprosczony remis
                    card_1 = cbuff_pop(A, &out_a, &len_a);
                    cbuff_push(card_1, A, out_a, &len_a);
                    card_2 = cbuff_pop(B, &out_b, &len_b);
                    cbuff_push(card_2, B, out_b, &len_b);
                }
                else{ //Wojna
                    turns = 0;
                    conflicts++; //za rozpoczęcie wojny odejmujemy 2 razy, dlatego trzeba to skorygować
                    switch(war(A, B, out_a, out_b, len_a, len_b, &conflicts, &turns)){
                        case OUT_OF_CONFLICTS: //Koniec konfliktów w trakcie
                            end_cond = OUT_OF_CONFLICTS;
                        break;
                        case OUT_OF_CARDS: //Koniec kart w trakcie
                            end_cond = OUT_OF_CARDS;
                        break;
                        case A_WINS: //Wygrał A
                            for(int i = 0; i < turns; i++){
                                card_1 = cbuff_pop(A, &out_a, &len_a);
                                cbuff_push(card_1, A, out_a, &len_a);
                            }
                            for(int i = 0; i < turns; i++){
                                card_2 = cbuff_pop(B, &out_b, &len_b);
                                cbuff_push(card_2, A, out_a, &len_a);
                            }
                        break;
                        case B_WINS: //Wygrał B
                            for(int i = 0; i < turns; i++){
                                card_1 = cbuff_pop(B, &out_b, &len_b);
                                cbuff_push(card_1, B, out_b, &len_b);
                            }
                            for(int i = 0; i < turns; i++){
                                card_2 = cbuff_pop(A, &out_a, &len_a);
                                cbuff_push(card_2, B, out_b, &len_b);
                            }
                        break;
                    }
                }
            break;
        }
        if(end_cond != 0) break;
        conflicts--;
        // cbuff_print(A, len_a, out_a);
        // printf("\n");
        // cbuff_print(B, len_b, out_b);
        // printf("\n");
    }
    switch(end_cond){
        case OUT_OF_CONFLICTS:
            printf("0\n");
            printf("%d\n%d", len_a, len_b);
        break;
        case OUT_OF_CARDS:
            printf("1\n");
            printf("%d\n%d", len_a, len_b);
        break;
        case A_WINS:
            battles = start-conflicts;
            printf("2\n");
            printf("%d", battles);
        break;
        case B_WINS:
            printf("3\n");
            cbuff_print(B, len_b, out_b);
        break;
    }




    return 0;
}

//-----------------------Funkcje-Gry---------------------
void split_cards(int full_deck[], int A[], int B[]){
    int n = CBUFF_SIZE;
    rand_permutation(n, full_deck);
    int deck_ind = 0;

    int i;
    for(i = 0; i < n/2; i++, deck_ind++){
        A[i] = full_deck[deck_ind];
    }
    for(i = 0; i < n/2; i++, deck_ind++){
        B[i] = full_deck[deck_ind];
    }
    return;
}

int conflict(int A[], int B[], int out_a, int out_b){
    if(A[out_a]/COLORS > B[out_b]/COLORS) return A_WINS; //A wygrał
    else if(A[out_a]/COLORS < B[out_b]/COLORS) return B_WINS; //B wygrał
    else return WAR; //wojna
}

void clear_arr(int arr[], int n){
    for(int i = 0; i < n; i++){
        arr[i] = 0;
    }
    return;
}

int war(int A[], int B[], int out_a, int out_b, int len_a, int len_b, int *conflicts, int *turns){
    // int stack_a[CBUFF_SIZE] = {0};
    // int stack_b[CBUFF_SIZE] = {0};
    // int curr_a = 0, curr_b = 0;

    while(1){
        if(len_a == 0 || len_b == 0) return OUT_OF_CARDS;
        switch(conflict(A, B, out_a, out_b)){
            case A_WINS:
                ++*turns;
                --*conflicts;
                return A_WINS;
            break;
            case B_WINS:
                ++*turns;
                --*conflicts;
                return B_WINS;
            break;
            case WAR:
                --*conflicts;
                if(*conflicts == 0) return OUT_OF_CONFLICTS;
                    out_a+=2;
                    out_a %= CBUFF_SIZE;
                    len_a-=2;

                    out_b+=2;
                    out_b %= CBUFF_SIZE;
                    len_b-=2;
                *turns+=2;
            break;
            }

    }

}

//------------------Funkcje-Kolejki---------------------------
// int cbuff[CBUFF_SIZE];
// int out=0, len=0;

double cbuff_push(int card, int cbuff[], int out, int *len){
    if(*len == CBUFF_SIZE) return INFINITY;

    cbuff[(out + *len)%CBUFF_SIZE] = card;
    ++*len;
    return 0.0;
}


int cbuff_pop(int cbuff[], int *out, int *len){
    if(len == 0) return -1;

    int leaving = cbuff[*out];
    --*len;
    ++*out;
    *out %= CBUFF_SIZE;
    return leaving;
}

// int cbuff_state(int cbuff[], int len){
//     return len;
// }

void cbuff_print(int cbuff[], int len, int out){
    int buff;
    for(int i = 0; i < len; i++){
        buff = (out+i)%CBUFF_SIZE;
        printf("%d ", cbuff[buff]);
    }
}


//------------------Funkcje-Losowania---------------------
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