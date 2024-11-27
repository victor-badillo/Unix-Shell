//Victor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#include "head_list.h"
#include "headers.h"

tMemList Lmem=NULL;

void Cmd_autores(char *tr[]){
    if (tr[0]==NULL){
        printf("logins -> p.legide@udc.es & victor.badillo@udc.es \n");
        printf("names  -> Pablo Legide & Victor Badillo \n");
        return;
    }
    if (!strcmp(tr[0], "-l"))
        printf("logins -> p.legide@udc.es & victor.badillo@udc.es \n");
    else if (!strcmp(tr[0],"-n"))
        printf("names  -> Pablo Legide & Victor Badillo \n");
}

void Cmd_pid(char *tr[]){
    if (tr[0]!=NULL && !strcmp(tr[0],"-p"))
        printf("Parent process pid: %d \n", getppid());
    else
        printf("Process pid: %d \n", getpid());
}

void Cmd_carpeta(char *tr[]){
    char dir[MAX];
    if (tr[0]==NULL)
        printf("%s \n", getcwd(dir,MAX));
    else if(chdir(tr[0]) == 0){
        printf("You changed of directory\n");
        printf("%s \n", getcwd(dir,MAX));
    }else if(chdir(tr[0])==-1){
        perror("Cannot change directory");
    }
}

void Cmd_fecha(char *tr[]){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    if (tr[0]==NULL)
        printf("%d/%d/%d %02d:%02d:%02d \n", tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900 ,  tm->tm_hour, tm->tm_min, tm->tm_sec);
    else if (!strcmp(tr[0], "-d"))
        printf("%d/%d/%d \n", tm->tm_mday, tm->tm_mon+1 , tm->tm_year+1900);
    else if (!strcmp(tr[0],"-h"))
        printf("%02d:%02d:%02d \n", tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void Cmd_hist(char *tr[], tList *L){
    int ncmd;

    if(tr[0] == NULL){
        printList(*L);
    }else if(!strcmp(tr[0],"-c")){
        freeList(L);
    }else{
        if(tr[0][0]=='-' && isNumberHist(tr[0])){
          ncmd=(int) abs(strtol(tr[0],NULL,10));
          printUntilN(*L,ncmd);
        }else printf("Please input a valid number for this command(it has to start with \"-\")\n");
    }
}

void Cmd_comando(char *tr[], tList L){

    char *trozos[MAX/2];
    char copy[MAX];
    char *sep;
    bool isInput=false;
    tItemL item; //called command item
    tItemL item2; //input's item
    
        
    if(tr[0] == NULL){
        printf("Please input a number of command\n");
        return;
    } 
       	
    if(!isNumberComm(tr[0])){ //in case it is not a number
        printf("Invalid number of command\n");
        return;
    }
    	
    int commandNumber; //input number    	    
    tPosL r=last(L); //input position
    commandNumber=atoi(tr[0]); //get the number after "comando". Convert the string to an integer
    
    if(commandNumber >= r->data.index){ //index out of bounds
        printf("Not valid number for this command\n");
        return;
    }
  	    
  	       
    item=getItem(commandNumber,L);
    strcpy(copy,item.comando);
 	       sep=strtok(copy, " \n\t"); //get the first word of called command
     	                  
    if(!strcmp(sep,"comando")){ //if the called command begins with the word "comando"
     	 item2=getItem(r->data.index,L);
     	 avoidInfiniteLoop(item.comando,commandNumber,isInput,item2,L);                         
    }else{ //otherwise
         TrocearCadena(item.comando,trozos, &L,isInput);
         ProcesarEntrada(trozos, &L);
    }          
}


void Cmd_infosis(char *tr[])
{
    struct utsname SysData;

    if(!uname(&SysData))
        printf("Node name: %s, Machine: %s\nOS info: \n-Name: %s\n-Release date: %s\n-Version:%s\n", SysData.nodename, SysData.machine, SysData.sysname, SysData.release, SysData.version);
}

void Cmd_ayuda(char *tr[])
{
    if(tr[0] == NULL){
        printf("-autores\n-pid\n-carpeta\n-fecha\n-hist\n-comando\n-infosis\n-ayuda\n-fin\n-salir\n-bye\n-create\n-stat\n-list\n-delete\n-deltree\n-allocate\n-deallocate\n-i-o\n-memdump\n-memfill\n-memory\n-recurse\n");
    }
    else if (!strcmp(tr[0],"autores")){
        printf("autores [-l][-n]: Prints the names and logins of the program authors. autores -l prints only the logins and authors -n prints only the names\n");
    }
    else if (!strcmp(tr[0],"pid")){
        printf("pid [-p]: Prints the pid of the process executing the shell. pid -p rints the pid of the shell`s parent process.\n");
    }
    else if (!strcmp(tr[0],"carpeta")){
        printf("carpeta [direct]: Changes the current working directory of the shell to direct(using the chdir system call).\n");
        printf("When invoked without arguments it prints the current working directory using the getcwd system call.\n");
    }
    else if (!strcmp(tr[0],"fecha")){
        printf("fecha [-d][-h]: It prints both the current date and the current time. fecha -d prints the current date in the format DD/MM/YYYY.");
        printf("fecha -h prints the current time in the format hh:mm:ss.\n");
    }
    else if (!strcmp(tr[0],"hist")){
        printf("hist [-c][-N]: Shows/clears the historic of commands executed by this shell.\n");
        printf("hist Prints all the commands that have been input with their order number\n");
        printf("hist -c Clears (empties) the list of historic commands\n");
        printf("hist -N prints the first N commands\n");
    }
    else if (!strcmp(tr[0],"comando")){
        printf("comando N: Repeats command number N (from historic list)\n");
    }
    else if (!strcmp(tr[0],"infosis")){
        printf("infosis: Prints information on the machine running the shell (as obtained via the uname system call/library function)\n");
    }
    else if (!strcmp(tr[0],"ayuda")){
        printf("ayuda [cmd]: displays a list of availables commands. ayuda cmd gives a brief\n");
        printf("help on the usage of command cmd\n");
    }
    else if (!strcmp(tr[0],"fin")){
        printf("fin: Ends the shell\n");
    }
    else if (!strcmp(tr[0],"salir")){
        printf("salir: Ends the shell\n");
    }
    else if (!strcmp(tr[0],"bye")){
        printf("bye: Ends the shell\n");
    }
    else if (!strcmp(tr[0],"create")){
    	printf("create [-f]: create files or directories. -f for creating files\n");
    }
    else if (!strcmp(tr[0],"stat")){
    	printf("stat [-long] [-link] [-acc] name1 name2 ... : gives information on files or ");
    	printf("directories\n -long gives more information about the file. -acc provides ");
    	printf("accesstime. -link provides the path if the link is symbolic\n");
    }
    else if (!strcmp(tr[0],"list")){
    	printf("list [-reca] [-recb] [-hid] [-long] [-link] [-acc] n1 n2 : list directories contents\n");
    	printf("\t-hid include hidden files. -reca recurisve(after). -recb recursive(before).\n");
    	printf("\tThe rest of parameters work the same way as if they were used with stat\n");
    }
    else if (!strcmp(tr[0],"delete")){
    	printf("delete [name 1 name 2 ..]: delete files and/or empty directories\n");
    }
    else if (!strcmp(tr[0],"deltree")){
    	printf("deltree [name1 name2 ..]: delete files and/or non empty directories recursively\n");
    }else if (!strcmp(tr[0],"allocate")){
    	printf("allocate [-malloc|-shared|-createshared|-mmap]... assigns a block of memory\n");
    	printf("\t-malloc tam: assigns a mmaloc block of size tam.\n\t-createshared cl tam: assigns (creating)");
    	printf("the shared block memory of key cl and size tam.\n\t-shared cl: assigns the block of memory shared");
    	printf("(already exists) of key cl.\n\t-mmap fich perm: maps the file fich and perm are the permissions\n");
    }
    else if (!strcmp(tr[0],"deallocate")){
    	printf("deallocate [-malloc|-shared|-delkey|-mmap|addr]... deassigns a block of memory\n");
    	printf("\t-malloc tam: deassigns a mmaloc block of size tam. -shared cl: deassigns the");
    	printf("block of memory shared of key cl\n\t-delkey cl: deletes from system (without demapping)");
    	printf("the key of memory cl.\n\t-mmap fich: demaps the file mapped fich. addr: deassigns the block of memory");
    	printf("in the address adr\n");
    }
    else if (!strcmp(tr[0],"i-o")){
    	printf("i-o [read|write] [-o] fiche addr cont : performs input output from disk to memory and viceversa\n");
    	printf("\tread fich addr cont: reads cont bytes from fich to addr\n\twrite [-o] fich addr cont: writes cont");
    	printf("bytes from addr to fich\n\t-o for overwriting. (addr is a memory adress)\n");
    }
    else if (!strcmp(tr[0],"memdump")){
    	printf("memdump addr cont: dumps on screen the contents (cont bytes) from addr memory position\n");
    }
    else if (!strcmp(tr[0],"memfill")){
    	printf("memfill addr cont byte: fill memory from addr with one character\n");
    }
    else if (!strcmp(tr[0],"memory")){
    	printf("memory [-blocks|-funcs|-vars|-all|-pmap] : shows info on the memory of the process\n");
    	printf("\t-blocks: blocks of memory assigned. -funcs: functions addresses. -vars: variables adresses\n");
    	printf("\t-all: everything. -pmap: output of command pmap(similar)\n");
    }
    else if (!strcmp(tr[0],"recurse")){
    	printf("recurse [n]: invokes the recursive function n times\n");
    }
}

void Cmd_fin(char *tr[])
{
    exit(0);
}


void Cmd_create(char *tr[]){
    
    char dir[MAX];
    
    if(tr[0]==NULL){
    	printf("%s\n",getcwd(dir,MAX));
    	printf("Please input a name for a file or directory\n");
    }else if(!strcmp(tr[0],"-f")){
        if(tr[1] == NULL) { 
          printf("%s\n",getcwd(dir,MAX)); 
          printf("Please indicate the name of the file\n"); 
        }else{        
    	  int ch= open(tr[1], O_CREAT | O_EXCL, 0775); //CREAT, create new files, EXCL avoid overwriting
    	
    	  if(ch == -1) perror("There was an error ");
    	  else printf("The file \"%s\" was created successfully\n",tr[1]);
    	}
    }else{
    	 if(mkdir(tr[0],0775) == -1) perror("There was an error ");
    	 else printf("The directory \"%s\" was created successfully\n",tr[0]);
    }
}

void Cmd_stat(char *tr[]){

    char dir[MAX];
    
    if(tr[0] == NULL){
    	printf("%s\n",getcwd(dir,MAX));
    	printf("Please input a name of a file or directory\n");
    }else{    
      struct statParams pr={0,0,0,0,0,0};
      int counterFiles=0;      
      pr=getParams(tr,pr);
      
      for(int i=0; tr[i]!=NULL ; i++){
      	if((strcmp(tr[i],"-long")) && (strcmp(tr[i],"-acc")) && (strcmp(tr[i],"-link"))){
      	    printStats(tr[i],&pr);
      	    counterFiles++;
      	}           	
      }
      if(counterFiles==0){ //if we type the command with parameters but there isn't any file
         printf("%s\n",getcwd(dir,MAX));
         printf("Please input a name of a file or directory\n");
      } 
   }    
}


void Cmd_list(char *tr[]){
    char dir[MAX];
    
    if(tr[0] == NULL){
    	printf("%s\n",getcwd(dir,MAX));
    	printf("Please input a name of a file or directory\n");
    }else{
    	struct statParams pr={0,0,0,0,0,0};
    	int counterFiles=0;   	
    	pr=getParams(tr,pr);

        for(int i=0; tr[i]!=NULL ; i++){
      
         if((strcmp(tr[i],"-long")) && (strcmp(tr[i],"-acc")) && (strcmp(tr[i],"-link")) && (strcmp(tr[i],"-hid")) && (strcmp(tr[i],"-reca")) && (strcmp(tr[i],"-recb")) ){
      	     printLstats(tr[i],&pr);
      	     counterFiles++;
         }           	
        }        
        if(counterFiles==0){ //if we type the command with parameters but there isn't any file
         printf("%s\n",getcwd(dir,MAX));
         printf("Please input a name of a file or directory\n");
        } 
    }    
}


void Cmd_delete(char *tr[]){

    char dir[MAX];
    
    if(tr[0] == NULL){
    	printf("%s\n",getcwd(dir,MAX));
    	printf("Please input a name of a file or an empty directory\n");
    }else{
    	for(int i=0; tr[i] != NULL; i++){
    	    if(!strcmp(tr[i],".") || !strcmp(tr[i],"..")) { printf("Avoid this operation(\".\" and \"..\")\n"); continue;}
    	    if(remove(tr[i]) != 0) //remove doesn't delete a file if it is opened and leave it as it was
     	      printf("Impossible deleting \"%s\": %s\n",tr[i],strerror(errno));
    	}
    }
}


void Cmd_deltree(char *tr[]){
  if(tr[0] == NULL){
        char dir[MAX];
        printf("%s\n",getcwd(dir,MAX));
        printf("Please input a name of a file or an empty directory\n");
  }else{
    struct stat fs;
    
    for(int i=0; tr[i]!=NULL ; i++){  
        if(!strcmp(tr[i],".") || !strcmp(tr[i],"..")) { printf("Avoid this operation(\".\" and \"..\")\n"); continue;}
        if (lstat(tr[i], &fs) == -1 )printf("Impossible deleting \"%s\": %s\n",tr[i],strerror(errno));//There is not such directory or file         
        else if (S_ISDIR(fs.st_mode)) { //deletes directories
           if(deleteInside(tr[i]) == -1) printf("Impossible deleting \"%s\": %s\n",tr[i],strerror(errno));
        }else remove(tr[i]); //deletes files
    }
  }
}


void Cmd_allocate(char *tr[]){
    if(tr[0]==NULL){    	
    	auxPrint();
    	return;
    }    
    if(!strcmp(tr[0],"-malloc")){
    	do_AllocateMalloc(tr);
    }
    else if(!strcmp(tr[0],"-shared")){
    	do_AllocateShared(tr);
    }
    else if(!strcmp(tr[0],"-createshared")){
    	do_AllocateCreateshared(tr);
    }
    else if(!strcmp(tr[0],"-mmap")){
    	do_AllocateMmap(tr);
    }
    else{
    	printf("allocate [-malloc|-shared|-createshared|-mmap] ....\n");
    }
    
    
}


void Cmd_deallocate(char *tr[]){
    
    if(tr[0] == NULL){
    	auxPrint();
    	return;
    }
    else if(!strcmp(tr[0],"-malloc")){
    	do_DeallocateMalloc(tr);
    }
    else if(!strcmp(tr[0],"-shared")){
    	do_DeallocateShared(tr);
    }
    else if(!strcmp(tr[0],"-delkey")){
        do_DeallocateDelkey (tr);
    }
    else if(!strcmp(tr[0],"-mmap")){
    	do_DeallocateMmap(tr);
    }
    else{
    	do_DeallocateAddress(tr);
    }
}



void Cmd_io(char *tr[]){
    if(tr[0] == NULL){
    	printf("Use: i-o [read|write] ......\n");
    	return;
    }
    
    if(!strcmp(tr[0],"read")){
    	do_I_O_read (tr);
    }
    else if(!strcmp(tr[0],"write")){
    	do_I_O_write(tr);
    }else{
    	printf("Use: i-o [read|write] ......\n");
    }
      
}



void Cmd_memdump(char *tr[]){

    size_t c = TAMLINE;		//deafult line=25
    
    if(tr[0]==NULL) return;
    
    if(tr[1] != NULL) c=(size_t) strtoul(tr[1],NULL,10);
    
    printf("Dumping %zu bytes from address %p\n",c,strToPointer(tr[0]));
    showMemory(strToPointer(tr[0]),c);
}


void Cmd_memfill(char *tr[]){

    size_t c;
    int n;

    if(tr[0] == NULL) return;    
    
    if(tr[1] == NULL){		//only address indicated
    	printf("Filling 128 bytes of memory with byte X(58) from address %p\n",strToPointer(tr[0]));
    	LlenarMemoria (strToPointer(tr[0]), (size_t) 128, (unsigned char) 'X');
    	return;
    }
    c=(size_t) strtoul(tr[1],NULL,10);		
    
    if(tr[2] == NULL){		//fill with default='X' with specified size
         printf("Filling %zu bytes of memory with byte X(58) from address %p\n",c,strToPointer(tr[0]));
         LlenarMemoria (strToPointer(tr[0]), c, (unsigned char) 'X');
         return;
    }

    if(tr[2][0] == 39 && tr[2][2]== 39 && (strlen(tr[2]) == 3) ){	//format: 'x'

         LlenarMemoria (strToPointer(tr[0]), c, (unsigned char) tr[2][1]);
         printf("Filling %zu bytes of memory with byte %c(%x) from address %p\n",c,(unsigned char) tr[2][1],(unsigned char) tr[2][1],strToPointer(tr[0]));

    }else{		//decimal or wrong input
         n=atoi(tr[2]);
         LlenarMemoria (strToPointer(tr[0]), c, (unsigned char) n);
         printf("Filling %zu bytes of memory with byte %c(%x) from address %p\n",c,n,n,strToPointer(tr[0]));
    }
}



void Cmd_memory(char *tr[]){	

    if(tr[0] == NULL || !strcmp(tr[0],"-all")){
    	printVars();
    	printFunctions();
    	auxPrint();
    }else if(!strcmp(tr[0],"-blocks")){
    	auxPrint();
    }else if(!strcmp(tr[0],"-funcs")){
    	printFunctions();
    }else if(!strcmp(tr[0],"-vars")){
    	printVars();
    }else if(!strcmp(tr[0],"-pmap")){
    	Do_pmap();
    }else{
    	printf("Not existing %s option\n",tr[0]);
    }
}


void Cmd_recurse(char *tr[]){

    int number;

    if(tr[0] == NULL){
    	printf("Please input a number\n");
    	return;
    }
    
    number=atoi(tr[0]);   
    Recursiva(number);
}


struct CMD c[]={
        {"autores",Cmd_autores},
        {"carpeta", Cmd_carpeta},
        {"pid",Cmd_pid},
        {"fecha",Cmd_fecha},
        {"infosis", Cmd_infosis},
        {"ayuda", Cmd_ayuda},
        {"fin",Cmd_fin},
        {"salir", Cmd_fin},
        {"bye",Cmd_fin},
        {"create",Cmd_create},
        {"stat",Cmd_stat},
        {"list",Cmd_list},
        {"delete",Cmd_delete},
        {"deltree",Cmd_deltree},
        {"allocate",Cmd_allocate},
        {"deallocate",Cmd_deallocate},
        {"i-o",Cmd_io},
        {"memdump",Cmd_memdump},
        {"memfill",Cmd_memfill},
        {"memory",Cmd_memory},
        {"recurse",Cmd_recurse},
        {NULL,NULL}
};

int TrocearCadena(char *cadena, char *trozos[], tList *L,bool isInput)
{
    int i=1;
    
    if(cadena[0]!='\n'){
        if(isInput) insertElement(cadena,L);
    }
    if ((trozos[0]=strtok(cadena, " \n\t"))==NULL)
        return 0;
    
    while((trozos[i]=strtok(NULL, " \n\t"))!=NULL)
        i++;
    return i;
}


void ProcesarEntrada(char *tr[], tList *L)
{
    int i;

    if (tr[0]==NULL)
        return;
    if(!strcmp("comando", tr[0])){
          Cmd_comando(tr+1,*L);
    }else if(!strcmp("hist", tr[0])){
        Cmd_hist(tr+1, L);

    }else {
        for(i=0;c[i].name!=NULL;i++){
            if (!strcmp(c[i].name,tr[0])){
                (*c[i].func)(tr+1);
                return;
            }

        }
        printf("Command not found \n");
    }
}

int main(int argc, char *argv[])
{
    char entrada[MAX];
    char *trozos[MAX/2];
    bool isInput=true; //decides if storing in the list
    tList L;
    createList(&L);
    createListMem(&Lmem);

    while (1){
        printf ("-->");
        fgets(entrada,MAX,stdin);        
        if (TrocearCadena(entrada,trozos, &L,isInput)==0)
            continue;        
        ProcesarEntrada(trozos, &L);
    }
}

