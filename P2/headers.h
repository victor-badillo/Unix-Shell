//Víctor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#define MAX 1024
#define _GNU_SOURCE
#define TAMANO 2048
#define TAMLINE 25


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/shm.h> 
#include <sys/wait.h>
#include <stddef.h>
#include <sys/mman.h>
#include "head_list.h"
#include "memory_list.h" 


struct CMD {
    char *name;
    void (*func)(char**);

};

struct statParams{
    int lon;
    int acc;
    int lnk;
    int hid;
    int reca;
    int recb;
};

int TrocearCadena(char *cadena, char *trozos[], tList *L,bool isInput);
void ProcesarEntrada(char *tr[], tList *L);

void Cmd_fin(char **);
void Cmd_autores(char **);
void Cmd_ayuda(char **);
void Cmd_carpeta(char **);
void Cmd_infosis(char **);
void Cmd_fecha(char **);
void Cmd_pid(char **);
void Cmd_create(char **);
void Cmd_stat(char **);
void Cmd_list(char **);
void Cmd_delete(char **);
void Cmd_deltree(char **);
void Cmd_allocate(char **);
void Cmd_deallocate(char **);
void Cmd_io(char **);
void Cmd_memdump(char **);
void Cmd_memfill(char **);
void Cmd_memory(char **);
void Cmd_recurse(char **);

char LetraTF(mode_t m);
char * ConvierteModo2(mode_t m);

struct statParams getParams(char *tr[], struct statParams pr);
bool isNumberHist(char* tr);
bool isNumberComm(char *str);
void avoidInfiniteLoop(char command[MAX],int commandNumber, bool isInput, tItemL item,tList L);
void traverseDirectory(char *tr);
void printStats(char *tr, struct statParams *pr);
int deleteInside(char *tr);
void printFiles(char * dir,struct statParams *pr);
void traverseIn(char *tr,struct statParams *pr);
void traverseOut(char *tr,struct statParams *pr, char * original);
void printLstats(char *tr, struct statParams *pr);
void auxPrintFiles(char *dir, char *name,struct statParams *pr);
int isDirectory(char * dir);
char * lastDir(char *directories);

void auxPrint();
void do_AllocateMalloc(char *tr[]);
void * ObtenerMemoriaShmget(key_t clave, size_t tam);
void do_AllocateCreateshared(char *tr[]);
void do_AllocateShared(char *tr[]);
void * MapearFichero (char * fichero, int protection);
void do_AllocateMmap(char *tr[]);
void do_DeallocateMalloc(char *tr[]);
void do_DeallocateShared(char *tr[]);
void do_DeallocateMmap(char *tr[]);
void deallocateWithAddress(tPosML p);
void * strToPointer(char *tr);
void do_DeallocateAddress(char *tr[]);
void showMemory( void *addr, size_t len); //
void printVars();
void printFunctions();
void Do_pmap (void);
void Recursiva (int n);
void LlenarMemoria (void *p, size_t cont, unsigned char byte);
ssize_t LeerFichero (char *f, void *p, size_t cont);
void do_I_O_read (char *ar[]);
ssize_t EscribirFichero(char *f, void *p, size_t cont,int overwrite);
void do_I_O_write(char *tr[]);
void do_DeallocateDelkey (char *args[]);

#define CONSTANT



