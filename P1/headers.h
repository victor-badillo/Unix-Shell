//Victor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#define MAX 1024
#define _GNU_SOURCE

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
#include "head_list.h"


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

#define CONSTANT



