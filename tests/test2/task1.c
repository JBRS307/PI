#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_ALLOCATION_ERROR -1

typedef char cbool;
#define true 1
#define false 0

typedef struct{
    char *name;
    char *surname;
    char pesel[12];
} Person;

typedef struct tagNode{
    Person *person;
    struct tagNode *next;
} PersonNode;

typedef struct{
    PersonNode *head; // pierwszy element
    PersonNode *tail; // ostatni element
} PersonList;

void init_pl(PersonList *p_pl){
    p_pl = (PersonList*)malloc(sizeof(PersonList));
    if(!p_pl)
        exit(MEMORY_ALLOCATION_ERROR);
    
    p_pl->head = NULL;
    p_pl->tail = NULL;
}

void free_person(Person *p){
    if(!p)
        return;
    if(p->name)
        free(p->name);
    if(p->surname)
        free(p->surname);
    free(p);
}

void free_pl(PersonList *p_pl){
    PersonNode *curr = p_pl->head;

    while(curr){
        PersonNode *next = curr->next;
        free_person(curr->person);
        free(curr);
        curr = next;
    }
}

void fill_person_data(Person *p_p, const char *pesel, const char *name, const char *surname){
    p_p = (Person*)malloc(sizeof(Person));
    if(!p_p)
        exit(MEMORY_ALLOCATION_ERROR);

    char *name_cp, *surname_cp;
    
    size_t name_len = strlen(name);
    name_cp = (char*)malloc(name_len+1);
    strcpy(name_cp, name);
    name_cp[name_len] = '\0';
    
    size_t surname_len = strlen(surname);
    surname_cp = (char*)malloc(surname_len+1);
    strcpy(surname_cp, surname);
    surname_cp[surname_len] = '\0';

    p_p->name = name_cp;
    p_p->surname = surname_cp;
    strcpy(p_p->pesel, pesel);
    p_p->pesel[11] = '\0';
}

int add(PersonList *p_pl, const char *pesel, const char *name, const char *surname){
    Person *p_p;
    fill_person_data(p_p, pesel, name, surname);

    PersonNode *new;
    PersonNode *curr = p_pl->head;
    if(!curr){
        new = (PersonNode*)malloc(sizeof(PersonNode));
        new->next = NULL;
        new->person = p_p;
        p_pl->head = new;
        p_pl->tail = p_pl->head;
        return 0;
    }

    while(curr){
        if(!strcmp(curr->person->pesel, p_p->pesel)){
            Person *old = curr->person;
            curr->person = p_p;
            free_person(old);
            return 1;
        }
    }
    
    new = (PersonNode*)malloc(sizeof(PersonNode));
    p_pl->tail->next = new;
    p_pl->tail = new;
    new->next = NULL;
    return 0;
}

Person *find(const PersonList *p_pl, const char *pesel){
    PersonNode *curr = p_pl->head;

    while(curr){
        if(!strcmp(curr->person->pesel, pesel))
            return curr->person
    }
    return NULL;
}

void merge(PersonList *p_target, const PersonList *p_src){
    PersonNode *curr = p_src->head;

    while(curr){
        Person *p = curr->person;
        add(p_target, p->pesel, p->name, p->pesel);
        curr = curr->next;
    }
}

int main(){

    return 0;
}