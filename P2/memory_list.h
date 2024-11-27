//Victor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#ifndef MEMORY_LIST_H
#define MEMORY_LIST_H

#include <stdbool.h>
#include <stddef.h>
#define MNULL NULL
#define MAX 1024


typedef struct tItemML{
    void* address;
    size_t size;
    struct tm *time;
    char *typeOfAllocation;
    int key;
    int fd;
    int index;
    char *nameOfFile;
    
}tItemML;

typedef struct tNodeM *tPosML;
struct tNodeM{
    tItemML data;
    tPosML next;
};
typedef struct tNodeM *tMemList;


void createListMem(tMemList *L);

bool isEmptyListMem(tMemList L);

tPosML firstMem(tMemList L);

tPosML lastMem(tMemList L);

tPosML nextMem(tPosML p, tMemList L);

bool insertNodeMem(tMemList *L, void *q, size_t s, struct tm *t,char *type, int k, int fileDescriptor, char *name);

void removeElementMem(tPosML p, tMemList *L);

void deleteAtPosition(tPosML p,tMemList *L);

void printListMem(tMemList L);

void printListMemMalloc(tMemList L);

void printListMemShared(tMemList L);

void printListMemMmap(tMemList L);

void freeListMem(tMemList *L);


#endif 
