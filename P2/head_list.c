//Victor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#include "head_list.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

bool createNode(tPosL *p){
    *p=malloc(sizeof (struct tNode));
    return (*p != LNULL);
}


void createList(tList *L){
    tPosL p;
    if(createNode(&p)){
    	*L= p;
    	(*L)->next=LNULL;
    }
    
}

bool isEmptyList(tList L){
    return (L->next==LNULL);
}

tPosL first(tList L){
    return L->next;
}

tPosL last(tList L){
    tPosL p;
    for(p=L->next;p->next!=LNULL;p=p->next);
    return p;
}

tPosL next(tPosL p, tList L){
    return(p->next);
}

bool insertElement(char data[], tList *L){
    tPosL m, p;
    if(!createNode(&m))return false;
    else{
        strcpy(m->data.comando, data);
        m->next=LNULL;
        m->data.index=0;
    }
    if((*L)->next==LNULL){
        (*L)->next=m;
        return true;
    }else{
        for(p=*L; p->next!=LNULL;p=p->next){ 
            m->data.index++;
        }
        p->next=m;
    
    return true;
   }
}

void removeElement(tPosL p, tList *L){
    
    if(p->next != LNULL){
        (*L)->next=p->next; 
    }
    if(p->next ==LNULL){
        (*L)->next=LNULL;
    }
    free(p);
      
}


void printList(tList L){
    tPosL p=L->next;
       
       while(p != LNULL){
            printf("%4d) %s",p->data.index,p->data.comando);
            p=p->next;
        }
    
}

void freeList(tList *L){
    tPosL p, aux;
    
    while((*L)->next != LNULL){
        p = (*L)->next;
        aux = p;
        p=p->next;
        removeElement(aux,L);
    }
}

void printUntilN(tList L, int n){
    tPosL p=L->next;

    while (p != LNULL){
    	if(p->data.index <= n)
        printf("%4d) %s",p->data.index,p->data.comando);
        p=p->next;
    }
}

tItemL getItem(int n, tList L){

    tPosL q;
    for(q=L->next;q->data.index!=n;q=q->next);
    return q->data;
}
