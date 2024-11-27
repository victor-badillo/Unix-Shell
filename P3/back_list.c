//Victor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#include "back_list.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>


bool createNodeBack(tPosBL *p){
    *p=malloc(sizeof (struct tNodeB));
    return (*p != BNULL);
}


void createListBack(tBList *L){
    tPosBL p;
    if(createNodeBack(&p)){
    	*L= p;
    	(*L)->next=BNULL;
    }
    
}


bool isEmptyListBack(tBList L){
    return (L->next==BNULL);
}

tPosBL firstBack(tBList L){
    return L->next;
}

tPosBL lastBack(tBList L){
    tPosBL p;
    for(p=L->next;p->next!=BNULL;p=p->next);
    return p;
}

tPosBL nextBack(tPosBL p, tBList L){
    return(p->next);
}

bool insertNodeBack(tBList *L, int pi, int u, char process[MAX],char st[MAX], char t[MAX], int o){
    tPosBL m, p;

    if(!createNodeBack(&m)) return false;
    else{
    	m->next=BNULL;
    	strcpy(m->data.tm,t);
    	m->data.pid=pi;
    	m->data.uid=u;
    	strcpy(m->data.pr, process);
    	strcpy(m->data.state,st);
    	m->data.out=o;
    	m->data.index=0;

    }
    if((*L)->next == BNULL){
    	(*L)->next=m;
    	return true;
    }else{
    	for(p=*L; p->next != BNULL; p=p->next){
    	   m->data.index++;
    	}
    	p->next=m;  	
    	return true;
    }
}


void removeElementBack(tPosBL p, tBList *L){
    
    if(p->next != BNULL){
        (*L)->next=p->next; 
    }
    if(p->next ==BNULL){
        (*L)->next=BNULL;
    }
    free(p);
      
}



void deleteAtPositionBack(tPosBL r,tBList *L){
    tPosBL p;
    
    if(r== ((*L)->next)){
    	(*L)->next= r->next;
    }
    else if(r->next == BNULL){
    	for(p=(*L)->next; p->next != r ; p=p->next);
    	p->next=BNULL;
    }
    else{
    	for(p=(*L)->next; p->next != r; p=p->next);
    	p->next=r->next;
    }
    
    free(r);
}

void freeListBack(tBList *L){
    tPosBL p, aux;
    
    while((*L)->next != BNULL){
        p = (*L)->next;
        aux = p;
        p=p->next;
        removeElementBack(aux,L);
    }
}

tItemBL getData(tPosBL p){
    return p->data;
}

void updateListBack(tPosBL p,tBList *L){

    if (waitpid(p->data.pid,&(p->data.out), WNOHANG |WUNTRACED |WCONTINUED) == p->data.pid){
            if(WIFEXITED(p->data.out)){                
                strcpy(p->data.state, "FINISHED");
                p->data.out = WEXITSTATUS(p->data.out);
            }else if(WIFSIGNALED(p->data.out)){
                strcpy(p->data.state, "SENALADO");
                p->data.out = WTERMSIG(p->data.out);
            }else if(WIFSTOPPED(p->data.out)){
                strcpy(p->data.state, "STOPPED");
                p->data.out = WTERMSIG(p->data.out);
            }else if(WIFCONTINUED(p->data.out)){
                strcpy(p->data.state, "ACTIVE");
                p->data.out = 0;
            }
                
        }
}





