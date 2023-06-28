#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

enum Vehicle_type {truck, car, v_ndef};

typedef struct{
    float load;
    short axes;
    int trailer_mass;
} Truck_data;

typedef struct{
    char vehicle_ID[15];
    unsigned int places:3;
    unsigned int driving_wheels:3;
    unsigned int towbar:1;
} Car_data;

typedef struct{
    int d, m, y;
} Date;

typedef struct{
    char owner[30];
    Date valid_date;
    enum Vehicle_type v_type;
    union{Car_data cs; Truck_data ts} veh_union;
} Vehicle;

int compar_date(const Date *a, const Date *b){
    int years = a->y - b->y;
    if(years == 0){
        int months = a->m - b->m;
        if(months == 0){
            return a->d - b ->d;
        }
        return months;
    }
    return years;
}

int delayed(const Vehicle *tv, int size, const Date *base_date, char ***pdelayed_owners){
    char **delayed_owners = (char**)malloc(sizeof(char*)*size);
    int delays = 0;
    int it = 0;
    for(int i = 0; i < size; i++){
        if(compar_date(&tv[i].valid_date, base_date) < 0){
            delayed_owners[it] = (char*)malloc(30);
            strcpy(delayed_owners[it], tv[i].owner);
            delays++;
        }
    }
    if(delays == 0){
        free(delayed_owners);
        pdelayed_owners = NULL;
        return 0;
    }
    pdelayed_owners = &delayed_owners;
    return delays;
}

void print_owners(const char **t_owners, size_t size){
    for(size_t i = 0; i < size; i++){
        printf("%s\n", t_owners[i]);
    }
}

void new_owner(Vehicle *v){
    char buff[1024];
    scanf("%s", buff);
    if(strlen(buff) > 30)
        return;
    
    strcpy(v->owner, buff);
}

void years_to_move(Date *d, int months){
    return months/12;
}

void move_date(Vehicle *v, int months){
    int years = years_to_move(v->valid_date, months);
}

int put_data(Vehicle *v, enum Vehicle_type type, const void *data){
    // zakładam, że type jest poprawne
    int res;
    if(v->v_type == v_ndef || v->v_type == type)
        res = 1;
    else
        res = 0;

    Car_data *car;
    Truck_data *truck;
    if(type == truck){
        truck = (Truck_data*)data;
        v->v_type = type;
        v->veh_union.ts = *truck;
    }else if(type == car){
        car = (Car_data*)data;
        v->v_type = type;
        v->veh_union.cs = *car;
    }
    return res;


}

int main(){

    return 0;
}