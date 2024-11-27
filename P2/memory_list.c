//Victor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#include "memory_list.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>


bool createNodeM(tPosML *p){
    *p=malloc(sizeof (struct tNodeM));
    return (*p != MNULL);
}


void createListMem(tMemList *L){
    tPosML p;
    if(createNodeM(&p)){
    	*L= p;
    	(*L)->next=MNULL;
    }
    
}


bool isEmptyListMem(tMemList L){
    return (L->next==MNULL);
}

tPosML firstMem(tMemList L){
    return L->next;
}

tPosML lastMem(tMemList L){
    tPosML p;
    for(p=L->next;p->next!=MNULL;p=p->next);
    return p;
}

tPosML nextMem(tPosML p, tMemList L){
    return(p->next);
}


bool insertNodeMem(tMemList *L, void *q, size_t s, struct tm *t,char *type, int k, int fileDescriptor, char *name){
    tPosML m, p;

    if(!createNodeM(&m)) return false;
    else{
    	m->next=MNULL;
    	m->data.address=q;
    	m->data.size=s;
    	m->data.time=t;
    	m->data.typeOfAllocation=type;
    	m->data.key=k;
    	m->data.fd=fileDescriptor;
    	m->data.nameOfFile=name;
    	m->data.index=0;
    }
    if((*L)->next == MNULL){
    	(*L)->next=m;
    	return true;
    }else{
    	for(p=*L; p->next != MNULL; p=p->next){
    	   m->data.index++;
    	}
    	p->next=m;  	
    	return true;
    }
}


void removeElementMem(tPosML p, tMemList *L){
    
    if(p->next != MNULL){
        (*L)->next=p->next; 
    }
    if(p->next ==MNULL){
        (*L)->next=MNULL;
    }
    free(p);
      
}



void deleteAtPosition(tPosML r,tMemList *L){
    tPosML p;
    
    if(r== ((*L)->next)){
    	(*L)->next= r->next;
    }
    else if(r->next == MNULL){
    	for(p=(*L)->next; p->next != r ; p=p->next);
    	p->next=MNULL;
    }
    else{
    	for(p=(*L)->next; p->next != r; p=p->next);
    	p->next=r->next;
    }
    
    if(!strcmp(r->data.typeOfAllocation,"malloc"))
    	free(r->data.address);
    
    if(!strcmp(r->data.typeOfAllocation,"descriptor"))
    	free(r->data.nameOfFile);
    
    free(r);
}


void printListMem(tMemList L){
    tPosML p=L->next;
    char t[MAX];
       
       while(p != MNULL){
            printf("\t%p%17lu ",p->data.address,p->data.size);
            
            strftime(t,MAX, "%b %d %H:%M ",p->data.time);
            printf("%s ",t);
            
            if(strcmp(p->data.typeOfAllocation,"descriptor"))
            	printf("%s",p->data.typeOfAllocation);
            
            if(!strcmp(p->data.typeOfAllocation,"shared")){
            	printf(" (key %d)",p->data.key);
            }
            if(!strcmp(p->data.typeOfAllocation,"descriptor")){
            	printf("%s  (%s %d)",p->data.nameOfFile,p->data.typeOfAllocation,p->data.fd);
            }
            
            printf("\n");
            p=p->next;
        }    
}


void printListMemMalloc(tMemList L){
    tPosML p=L->next;
    char t[MAX];
       
       while(p != MNULL){
       	    if(!strcmp(p->data.typeOfAllocation,"malloc")){
       	    	printf("\t%p%17lu ",p->data.address,p->data.size);
            	strftime(t,MAX, "%b %d %H:%M ",p->data.time);
            	printf("%s %s \n",t,p->data.typeOfAllocation);
       	    }            
            p=p->next;
        }   
}


void printListMemShared(tMemList L){
    tPosML p=L->next;
    char t[MAX];
       
       while(p != MNULL){
       	    if(!strcmp(p->data.typeOfAllocation,"shared")){
       	    	printf("\t%p%17lu ",p->data.address,p->data.size);
            	strftime(t,MAX, "%b %d %H:%M ",p->data.time);
            	printf("%s %s ",t,p->data.typeOfAllocation);
            	printf("(key %d)\n",p->data.key);
       	    }            
            p=p->next;
        }   
}


void printListMemMmap(tMemList L){
    tPosML p=L->next;
    char t[MAX];
       
       while(p != MNULL){
       	    if(!strcmp(p->data.typeOfAllocation,"descriptor")){
       	    	printf("\t%p%17lu ",p->data.address,p->data.size);
            	strftime(t,MAX, "%b %d %H:%M ",p->data.time);
            	printf("%s ",t);
            	printf("%s  (%s %d)\n",p->data.nameOfFile,p->data.typeOfAllocation,p->data.fd);
            	
       	    }            
            p=p->next;
        }   
}

void freeListMem(tMemList *L){
    tPosML p, aux;
    
    while((*L)->next != MNULL){
        p = (*L)->next;
        aux = p;
        p=p->next;
        removeElementMem(aux,L);
    }
}


