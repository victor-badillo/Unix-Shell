//Victor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#ifndef ASSIG1_LIST_H
#define ASSIG1_LIST_H

#include <stdbool.h>
#define LNULL NULL
#define MAX 1024


typedef struct tItemL{
    char comando[MAX];
    int index;
}tItemL;

typedef struct tNode *tPosL;
struct tNode{
    tItemL data;
    tPosL next;
};
typedef struct tNode *tList;


void createList(tList *L);

bool isEmptyList(tList L);

tPosL first(tList L);

tPosL last(tList L);

tPosL next(tPosL p, tList L);

bool insertElement(char data[], tList *L);

void removeElement(tPosL p, tList *L);

void printList(tList L);

void freeList(tList *L);

void printUntilN(tList L, int n);

tItemL getItem(int n, tList L);


#endif //ASSIG1_LIST_H

