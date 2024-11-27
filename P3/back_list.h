//Victor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#ifndef BACK_LIST_H
#define BACK_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BNULL NULL
#define MAX 1024


typedef struct tItemBL{
    pid_t pid;
    int uid;
    char pr[MAX];
    char state[MAX];
    char tm[MAX];
    int index;
    int out;
    
}tItemBL;

typedef struct tNodeB *tPosBL;
struct tNodeB{
    tItemBL data;
    tPosBL next;
};
typedef struct tNodeB *tBList;


void createListBack(tBList *L);

bool isEmptyListBack(tBList L);

tPosBL firstBack(tBList L);

tPosBL lastBack(tBList L);

tPosBL nextBack(tPosBL p, tBList L);

bool insertNodeBack(tBList *L, int pid, int uid, char pr[MAX],char state[MAX],char tm[MAX],int out);

void removeElementBack(tPosBL p, tBList *L);

void deleteAtPositionBack(tPosBL p,tBList *L);

void freeListBack(tBList *L);

tItemBL getData(tPosBL p);

void updateListBack(tPosBL p,tBList *L);


#endif 
