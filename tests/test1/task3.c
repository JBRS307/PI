#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024

typedef enum {electric, hybrid, combustion} EngineType;
typedef enum {car, bus, truck} VehicleType;

typedef union{
    int people; // samochód
    int seats; // autobus
    float capacity; // tiry
} Specific1;

typedef union{
    float power; // samochód
    int standing; // autobus
    float pressure; // tiry
} Specific2;

typedef struct{
    char *owner;
    int ov_date[3]; // d, m, y
    VehicleType vtype;
    EngineType engine;
    Specific1 spec1;
    Specific2 spec2;
} Vehicle;

void new_owner(Vehicle *pvehicle){
    char buff[BUFF_SIZE]; // Nie istnieją nazwiska lub firmy dłuższe niż to

    scanf("%s", buff);
    size_t len = strlen(buff);

    char *new_owner = (char*)malloc(len+1);
    strcpy(new_owner, buff);
    new_owner[len] = '\0';

    char *to_delete = pvehicle->owner;
    free(to_delete);

    pvehicle->owner = new_owner;
}

int compar_date(const int *d1, const int *d2){
    int y = d2[2]-d1[2];
    if(y == 0){
        int m = d2[1]-d1[1];
        if(m == 0){
            return d2[0]-d1[0];
        }
        return m;
    }
    return y;
}

int delayed(Vehicle *pvehicle, int size, int *base_date, char ***pdelayed_owners){
    char **delayed_owners = (char**)malloc(sizeof(char*)*size);
    int it = 0;
    for(int i = 0; i < size; i++){
        int *date = pvehicle[i].ov_date;
        if(compar_date(date, base_date) > 0){
            delayed_owners[it] = pvehicle[i].owner;
            it++;
        }
    }
    if(it == 0){
        free(delayed_owners);
        pdelayed_owners = NULL;
    }else{
        delayed_owners = (char**)realloc(delayed_owners, sizeof(char*)*it);
        pdelayed_owners = &delayed_owners;
    }
    return it;
}

int main(){


    return 0;
}