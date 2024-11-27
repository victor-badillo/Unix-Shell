//Victor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#include "headers.h"

extern tMemList Lmem;

//extern tBList Lback;

extern char ** env1; 

float p1=1.5;
float p2=3.75;
float p3=5.25;


//Only numbers starting with "-"
bool isNumberHist(char* str){
   for(int j=0; j < strlen(str) ; j++){
      if(str[j]<48 || str[j]>57){
         
         if(str[j] != 45 && j>0){ //45 is "-" sign in ASCII table
           return false;
         }
         if(j>0 && str[j]==45) return false; //avoid negative numbers with minus sign within it
      }
   }
   return true;
}

//Only positive numbers
bool isNumberComm(char* str){
   for(int j=0; j < strlen(str) ; j++){
      if(j > 0 && str[j]==45)
      	    return false;
      if(str[j]<48 || str[j]>57){
            return false;
       }
   }  
   return true;
}


//Set 1 to each parameter if it is input
struct statParams getParams(char *tr[], struct statParams pr){

   for(int i=0; tr[i] != NULL; i++){
        if (!strcmp(tr[i],"-long")) 	 pr.lon=1;
        else if (!strcmp(tr[i],"-acc"))  pr.acc=1;
        else if (!strcmp(tr[i],"-link")) pr.lnk=1;
        else if (!strcmp(tr[i],"-hid"))  pr.hid=1;
        else if (!strcmp(tr[i],"-reca")) pr.reca=1;
        else if (!strcmp(tr[i],"-recb")) pr.recb=1;
   }
   return pr;
}

void avoidInfiniteLoop(char command[MAX],int commandNumber, bool isInput, tItemL item,tList L){

    int commandNumberTrozos; //number's called command
    char *trozos[MAX/2];
    char copy[MAX];
    char *token;
    
    strcpy(copy,command);
    token=strtok(copy," \n\t");
    
    if( (token=strtok(NULL," \n\t")) != NULL){  //called command has a parameter             
        if(isNumberComm(token)){ //called command parameter is a number
            commandNumberTrozos=atoi(token);
            
            if(commandNumberTrozos == commandNumber || commandNumberTrozos == item.index) //prevent calling itself and among them
               printf("Impossible, infinite loop\n")  ; 
            else{
               TrocearCadena(command,trozos, &L,isInput);
               ProcesarEntrada(trozos, &L);
            }        
        }else printf("Invalid number of command\n");
    }else  printf("Please input a number of command\n");
}

char LetraTF (mode_t m)
{
     switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */ 
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
     }
}


char * ConvierteModo2 (mode_t m)
{
    static char permisos[12];
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}  


void printStats(char *tr, struct statParams *pr){
    struct stat fs; 
    // lstad instead of stat because if path is a symbolic link,then the link itslef is stat-ed, not the file    
    char copy[MAX];
    strcpy(copy,tr);
    
    if(lstat(tr,&fs)){ perror("There was an error"); return;}
    
    int total= pr->lon + pr->lnk + pr->acc + pr->hid + pr->reca + pr->recb;
    
    if(total==0){    	
        printf("% 8ld  %s\n", fs.st_size,lastDir(tr));
    }else{
       	
    	//-long option
    	if(pr->lon){
    	
    	    struct group *gr;
            struct passwd *pw;
            char buffer[MAX];
            char symBuff[MAX]="";  //initialize to "" for avoiding memory errors
            char *user,*group;
    
    	    //if((pw=getpwuid(fs.st_uid)) == NULL) perror("There was an error ");
    	    user=((pw=getpwuid(fs.st_uid)) == NULL )? "?????" : pw->pw_name;
    	    group=((gr=getgrgid(fs.st_gid)) == NULL ) ? "?????" : gr->gr_name;
    	    //if((gr=getgrgid(fs.st_gid)) == NULL) perror("There was an error ");
    	
    	    //-acc option
    	    if((pr->acc) == 1) strftime(buffer,MAX,"%Y/%m/%d-%H:%M",localtime(&fs.st_atime));
    	    else strftime(buffer,MAX,"%Y/%m/%d-%H:%M",localtime(&fs.st_mtime));
    	 	
    	     printf("%s   %ld (  %ld)   %s   %s",buffer,fs.st_nlink,fs.st_ino,user,group);
    	     printf(" %s\t% 8ld %s",ConvierteModo2(fs.st_mode),fs.st_size,lastDir(tr));
       	    
    	    //-link option
    	    if(!pr->lnk){
    	       printf("\n");
    	    }else{
    	       if(pr->lnk && (readlink(copy,symBuff,MAX) != -1))  
    	          printf(" ->%s\n",symBuff);
    	       else printf(" ->\n");
    	    }
         	    
        }else printf("% 8ld  %s\n", fs.st_size,lastDir(tr));
   }
}

int deleteInside(char *tr){

    DIR *direct;
    struct dirent *d;
    char aux[MAX];
    int r=-1;
    
    if((direct=opendir(tr)) != NULL){
           
        r=0;
        while (!r &&  (d = readdir (direct)) != NULL){ //iterate through directory
           
            int r2=-1;
            if(!strcmp(d->d_name, "..") || !strcmp(d->d_name, ".")) continue; // skip "." and ".."            
            snprintf(aux, MAX, "%s/%s", tr, d->d_name); //concatenate directory and inner file or directory
            
            if(isDirectory(aux))
                    r2=deleteInside(aux);           
            else r2=unlink(aux);
            r=r2;
        }
        closedir(direct);
    }else perror("There was an error ");

    if (!r)
     r = rmdir(tr);
    return r;
}

int isDirectory(char * dir){
    struct stat fs;

    if(lstat(dir, &fs)==-1){
        return 0;
    }
    return S_ISDIR(fs.st_mode);
}


char * lastDir(char *directories){
    char *dirs[MAX/2];
    int i=1;
    
    dirs[0]=strtok(directories, "/ ");
    while((dirs[i]=strtok(NULL, "/ "))!=NULL)
        i++;
    if(i==1) return dirs[0]; //only one token
    else return dirs[i-1];   //otherwise, last one
}


void auxPrintFiles(char *dir, char *name,struct statParams *pr){
    struct stat fs;
    char aux[MAX];
    
    snprintf(aux, MAX, "%s/%s", dir, name); //concatenate directory and inner file or directory

    if(!lstat(aux, &fs)){           
        if(pr->lon) printStats(aux,pr); //if long option is included, we print stats in that format
        else printf("%10ld  %s\n",fs.st_size, name);
    }else printf("%s\n", strerror(errno));             
}


void printFiles(char * dir, struct statParams *pr){

    DIR *direct;
    struct dirent *d; 
    
    if((direct=opendir(dir)) != NULL){
      
        while ((d = readdir (direct)) != NULL){ //iterate through directory        
           if(!(pr->hid)){ 
           
             if(!strcmp(d->d_name, "..") || !strcmp(d->d_name, ".")) continue; // skip "." and ".."
             if(!(pr->hid) && d->d_name[0]=='.') continue; //in case there is not -hid
           
             auxPrintFiles(dir,d->d_name,pr);
           }else auxPrintFiles(dir,d->d_name,pr);
       }
        closedir(direct);
   }   
}


void traverseIn(char *tr,struct statParams *pr){ //auxiliary function for -reca
 
    DIR *direct;
    struct dirent *d;
    char aux[MAX];
    
    if((direct=opendir(tr)) != NULL){
   
          printf("************%s\n", tr);
          printFiles(tr,pr);
          
        while ((d = readdir (direct)) != NULL){ //iterate through directory

            if(!(pr->hid) && d->d_name[0]=='.') continue; //in case there is not -hid
            if(!strcmp(d->d_name, "..") || !strcmp(d->d_name, ".")) continue;// skip "." and ".."
            
            snprintf(aux, MAX, "%s/%s", tr, d->d_name); //concatenate directory and inner file or directory

            if(isDirectory(aux))
                traverseIn(aux,pr);                             
        }
        closedir(direct);
    }
}


void traverseOut(char *tr,struct statParams *pr, char * original){ //auxiliary function for -recb

    DIR *direct;
    struct dirent *d;
    char aux[MAX];

    if((direct=opendir(tr)) != NULL){
    
        while ((d = readdir (direct)) != NULL){ //iterate through directory               
            
            if(!strcmp(d->d_name, "..") || !strcmp(d->d_name, "."))continue;// skip "." and ".."
            
            snprintf(aux, MAX, "%s/%s", tr, d->d_name); //concatenate directory and inner file or directory

            if(isDirectory(aux)){
                  traverseOut(aux,pr,original);
                  if(pr->hid || d->d_name[0]!='.'){ //print if the file is not hidden and hid is input
                      printf("************%s\n", aux);
                      printFiles(aux,pr);
                   }               
            }
        }
        closedir(direct);
    }else{
        perror("There was an error ");
    }
    if(!strcmp(tr, original)){ //print stats of first directory        
        printf("************%s\n", original);
        printFiles(original,pr);
    }
}


void printLstats(char *tr, struct statParams *pr){
    struct stat fs;      
    //lstad instead of stat because if path is a symbolic link,then the link itslef is stat-ed, not the file
    if(lstat(tr,&fs)){ perror("There was an error"); return;}
    int total= pr->lon + pr->lnk + pr->acc + pr->hid + pr->reca + pr->recb;
    
    if(total==0){
        if(S_ISDIR(fs.st_mode)){
           printf("************%s\n", tr);
           printFiles(tr,pr);
        }else printf("%10ld  %s\n",fs.st_size, lastDir(tr));       
    }else if(!(S_ISDIR(fs.st_mode))){
        printStats(tr,pr);
    }else{
             
        if((pr->recb) == 1){ //-recb option (major priority)
            char copy[MAX];
            strcpy(copy,tr);
            traverseOut(tr,pr,copy);                    
        }else if(pr->reca && !pr->recb){ //-reca option
            traverseIn(tr,pr);
        }else printFiles(tr,pr); 
    }
}


void auxPrint(){
   printf("******List of assigned blocks for process %d\n", getpid());
   if(!isEmptyListMem(Lmem))
   	printListMem(Lmem);
}


void do_AllocateMalloc(char *tr[]){
   size_t tam;
   void *p;
     
   if(tr[1]==NULL){		
   	printf("******List of malloc assigned blocks for process %d\n", getpid());
   	printListMemMalloc(Lmem);
   	return;
   }
   
   tam=(size_t) strtoul(tr[1],NULL,10);
   if(tam == 0){		//wrong input
   	printf("0 byte blocks are not assigned\n");
   	return;
   }
   
   if((p=(void *) malloc(tam*sizeof(tam))) == NULL ){
   	perror("Impossible to obtain memory with malloc");
   }
   else{
   	time_t t= time(NULL);
        struct tm *tm = localtime(&t);

   	if(insertNodeMem(&Lmem, p, tam, tm,"malloc",0,0,"")){		//insert node in the list
   		printf("Assigned %lu bytes in %p\n",(unsigned long)tam,p);
   	}else{
   		printf("Not possible to insert in the list of memory blocks\n");
   	}
   	
   }
}


void * ObtenerMemoriaShmget (key_t clave, size_t tam){
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;
    

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)  /*no nos vale*/
        {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
             shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
    time_t t= time(NULL);
    struct tm *tm = localtime(&t);

    if(!insertNodeMem(&Lmem, p, s.shm_segsz, tm,"shared",(int) clave,0,"")){
    	printf("Not possible to insert in the list of memory blocks\n");
    }
    if(tam==0){
    	printf("Memory shared of key %d in %p\n",(int) clave, p);
    }
    
    return (p);
}


void do_AllocateCreateshared (char *tr[]){
   key_t cl;
   size_t tam;
   void *p;
   
   if (tr[1]==NULL || tr[2]==NULL) {
   
   	printf("******List of shared assigned blocks for process %d\n", getpid());
   	if(!isEmptyListMem(Lmem))
   		printListMemShared(Lmem);
	return;
   }
   if(!isNumberComm(tr[1])){		//invalid key
   	printf("Not valid key\n");
   	return;
   }
   cl=(key_t)  strtoul(tr[1],NULL,10);
   tam=(size_t) strtoul(tr[2],NULL,10);
   if (tam==0) {			//key=0
	printf ("0 bytes blocks are not assigned\n");
	return;
   }
   if ((p=ObtenerMemoriaShmget(cl,tam))!=NULL)
		printf ("%lu bytes assigned in %p\n",(unsigned long) tam, p);
   else
		printf ("Impossible to assign shared memory key %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void do_AllocateShared(char *tr[]){
   
   key_t cl;
   size_t tam=0;	//indicates avoiding creating new one
   void *p;
   
   if(tr[1] == NULL){
   	printf("******List of shared assigned blocks for process %d\n", getpid());
   	printListMemShared(Lmem);
   	return;
   }
   cl=(key_t)  strtoul(tr[1],NULL,10);
		
   if ((p=ObtenerMemoriaShmget(cl,tam))==NULL)
		printf ("Impossible to assign shared memory key %lu:%s\n",(unsigned long) cl,strerror(errno));
		  
}


void * MapearFichero (char * fichero, int protection){
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;
    
    char *copy=strdup(fichero);		//copy for saving the file name
    
    if (protection&PROT_WRITE)
          modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1){
    	  free(copy);
          return NULL;
    }
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED){
           free(copy);
           return NULL;
    }

    time_t t= time(NULL);
    struct tm *tm = localtime(&t);
      
    insertNodeMem(&Lmem, p, (size_t)s.st_size, tm,"descriptor",0,df,copy);	//insert node in the list
    
    return p;
}


void do_AllocateMmap(char *tr[]){ 
     char *perm;
     void *p;
     int protection=0;
     
     if (tr[1]==NULL){ 
      	
   	printf("******List of mmap assigned blocks for process %d\n", getpid());
   	if(!isEmptyListMem(Lmem))
     		printListMemMmap(Lmem); 
     	return;
     }
     if ((perm=tr[2])!=NULL && strlen(perm)<4) {
            if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
            if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
            if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
     }
     if ((p=MapearFichero(tr[1],protection))==NULL)
             perror ("Impossible mapping file");
     else
             printf ("%s file mapped in %p\n", tr[1], p);
}



void do_DeallocateMalloc(char *tr[]){
    
    tPosML r=NULL;
    tPosML p;
    size_t num;
    
    	
    if(tr[1] == NULL){  	
   	printf("******List of malloc assigned blocks for process %d\n", getpid());
   	if(!isEmptyListMem(Lmem))
     		printListMemMalloc(Lmem); 
     	return;
    }
    if(!isEmptyListMem(Lmem)){
    	
    	if(!isNumberComm(tr[1])){	//not a positive number
    	   printf("There is not any block of indicated size assigned with malloc\n");
    	   return;
    	}
    	num= (size_t) atoi(tr[1]);
    	
    	for(p=(Lmem)->next; p != MNULL; p=p->next){	//find malloc with indicated size
    	   if((p->data.size) == num)
    	   	r=p;
    	}
    	   	
    	if(r != NULL){
    		deleteAtPosition(r,&Lmem);
    	}
    	else printf("There is not any block of indicated size assigned with malloc\n");
    	  	
    }else  printf("There is not any block of indicated size assigned with malloc\n");
    
}


void do_DeallocateShared(char *tr[]){

    int num;
    tPosML p;

    if(tr[1] == NULL){

   	printf("******List of shared assigned blocks for process %d\n", getpid());
   	if(!isEmptyListMem(Lmem))
   		printListMemShared(Lmem);
	return;
    }
     if(!isEmptyListMem(Lmem)){
    	
    	if(!isNumberComm(tr[1]) || !strcmp(tr[1],"0")){		//not valid key
    	   printf("There is not existing block with indicated key\n");
    	   return;
    	}
    	num=atoi(tr[1]);
    	
    	for(p=(Lmem)->next; p != MNULL; p=p->next){		//find shared with indicated key
    	   if((p->data.key) == num){
    	   	(shmdt(p->data.address) != 0)? perror("There was en error:") : deleteAtPosition(p,&Lmem);
    	   	return;
    	   }
    	}
        printf("There is not existing block with indicated key\n");
    	
    	  	
    }else  printf("There is not existing block with indicated key\n");
    
}


void do_DeallocateMmap(char *tr[]){

    tPosML r=NULL;
    tPosML p;
    
    if(tr[1] == NULL){

   	printf("******List of mmap assigned blocks for process %d\n", getpid());
   	if(!isEmptyListMem(Lmem))
   		printListMemMmap(Lmem);
	return;
    }
    
    if(!isEmptyListMem(Lmem)){
    	
    	for(p=(Lmem)->next; p != MNULL; p=p->next){	//find mmap with indicated name
    	   if(!strcmp(tr[1],p->data.nameOfFile)){
    	        r=p; 
    	   	if((munmap(p->data.address,p->data.size)) == -1)
    	   		perror("There was an error");
    	   		
    		deleteAtPosition(r,&Lmem);
    	   	return;
    	   }
    	}
  	printf("Not mapped file %s\n",tr[1]);
  		
    }else  printf("Not mapped file %s\n",tr[1]);
    
}


void deallocateWithAddress(tPosML p){
    
    if(!strcmp(p->data.typeOfAllocation,"malloc")){
    	deleteAtPosition(p,&Lmem);
    }
    else if(!strcmp(p->data.typeOfAllocation,"shared")){
    	(shmdt(p->data.address) != 0)? perror("There was en error:") : deleteAtPosition(p,&Lmem);
    }
    else{
    	if((munmap(p->data.address,p->data.size)) == -1)
    	   	perror("There was an error");
    	   		
    	deleteAtPosition(p,&Lmem);
    }
}


void * strToPointer(char *tr){		//converts a string into a pointer
    void *p;
    sscanf(tr,"%p",&p);
    return p;
}


void do_DeallocateAddress(char *tr[]){
    
    void *r = strToPointer(tr[0]);
    tPosML p;  
    
    if(isEmptyListMem(Lmem)){
        printf("Not assigned address %p with malloc, shared or mmap\n",r);
    	return;
    }
    for(p=(Lmem)->next; p != MNULL; p=p->next){		//find address in the list
    	
    	if(r == p->data.address){
    		deallocateWithAddress(p);
    		return;
    	}
    }
   
    printf("Not assigned address %p with malloc, shared or mmap\n",r);
}



void showMemory( void *addr, size_t len){  
    size_t i;
    size_t j;    
    unsigned char *pc = (unsigned char *)addr; 

    for(i=1; i<=len ; i++){
        if(pc[i-1] != '\0')  printf(" %c ",pc[i-1]);	//print characer
        else   printf("   ");				//find null ,then no character
        
        if(i%25 == 0){					//line size 25
            printf("\n");
            for(j=i-25; j<i; j++){			//print hex code for character
                printf("%.2x ", pc[j]);
            }
            printf("\n");
        }
    }
    printf("\n");
    if((len%25) != 0){					//print last line if is not multiple of 25
        for(size_t k=len-(len%25)+1; k<=len; k++){
                printf("%.2x ", pc[k-1]);
        }
        printf("\n");
    }

}




void printVars(){
    int i=1,j=2,k=3;
    static double pi= 3.141592;
    static double contr = 1.2345;
    static double r = 5.4321;
    
    printf("Local variables%24p,    %p,    %p\n",&i,&j,&k);
    printf("Global variables%23p,    %p,    %p\n",&p1,&p2,&p3);
    printf("Static variables%23p,    %p,    %p\n",&pi,&contr,&r);
}


void printFunctions(){
    printf("Program functions%22p,    %p,    %p\n",printVars,printFunctions,Do_pmap);
    printf("Library functions%22p,    %p,    %p\n",printf,scanf,strcat);
}

void Do_pmap (void) /*sin argumentos*/
 { pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
   char elpid[32];
   char *argv[4]={"pmap",elpid,NULL};
   
   sprintf (elpid,"%d", (int) getpid());
   if ((pid=fork())==-1){
      perror ("Imposible creating process");
      return;
      }
   if (pid==0){
      if (execvp(argv[0],argv)==-1)
         perror("cannot execute pmap (linux, solaris)");
         
      argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;   
      if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
         perror("cannot execute procstat (FreeBSD)");
         
      argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;    
            if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
         perror("cannot execute procmap (OpenBSD)");
         
      argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
      if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
         perror("cannot execute vmmap (Mac-OS)");      
      exit(1);
  }
  waitpid (pid,NULL,0);
}


void Recursiva (int n){
  char automatico[TAMANO];
  static char estatico[TAMANO];

  printf ("parameter:%3d(%p) array %p, static arr %p\n",n,&n,automatico, estatico);

  if (n>0)
    Recursiva(n-1);
}

void do_DeallocateDelkey (char *args[])
{
   key_t clave;
   int id;
   char *key=args[1];

   if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey needs valid_key\n");
        return;
   }
   if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
   }
   if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}


void LlenarMemoria (void *p, size_t cont, unsigned char byte){
  unsigned char *arr=(unsigned char *) p;
  size_t i;

  for (i=0; i<cont;i++)
		arr[i]=byte;
}


ssize_t LeerFichero (char *f, void *p, size_t cont){
   struct stat s;
   ssize_t  n;  
   int df,aux;

   if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
	return -1;     
   if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
	cont=s.st_size;
   if ((n=read(df,p,cont))==-1){
	aux=errno;
	close(df);
	errno=aux;
	return -1;
   }
   close (df);
   return n;
}


void do_I_O_read (char *ar[]){
   void *p;
   size_t cont=-1;
   ssize_t n;
   if (ar[1]==NULL || ar[2]==NULL){
	printf ("Parameters are missing\n");
	return;
   }
   p=strToPointer(ar[2]);  /*convertimos de cadena a puntero*/
   if (ar[3]!=NULL)
	cont=(size_t) atoll(ar[3]);

   if ((n=LeerFichero(ar[1],p,cont))==-1)
	perror ("Impossible to read file");
   else
	printf ("%lld bytes read from %s in %p\n",(long long) n,ar[1],p);
}


ssize_t EscribirFichero(char *f, void *p, size_t cont,int overwrite){
   ssize_t  n;
   int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;

   if (overwrite)
	flags=O_CREAT | O_WRONLY | O_TRUNC;
   
   if ((df=open(f,flags,0777))==-1)
	return -1;

   if ((n=write(df,p,cont))==-1){
	aux=errno;
	close(df);
	errno=aux;
	return -1;
   }
   close (df);
   return n;
}


void do_I_O_write(char *tr[]){
    
    ssize_t n;
    size_t cont;
    
    if(tr[1] == NULL){
    	printf("Parameters are missing\n");
    	return;
    }
    
    if(!strcmp(tr[1],"-o")){			//overwriting option
    	if(tr[2] == NULL || tr[3] == NULL || tr[4] == NULL){	//insufficient characters
    	    printf("Parameters are missing\n");
    	    return;
    	}
    	cont =strtoul (tr[4],NULL,10);

    	if((n=EscribirFichero(tr[2], strToPointer(tr[3]), cont,1)) == -1)	//last parameter=1 for overwriting
    	    perror("There was an error");
    	else printf ("%s bytes written in %s from %p\n",tr[4],tr[2],strToPointer(tr[3]));
    }
    else{
    	if(tr[1] == NULL || tr[2] == NULL || tr[3] == NULL){	//insufficient characters
    	    printf("Parameters are missing\n");
    	    return;
    	}
    	cont =strtoul (tr[3],NULL,10);
    	if((n=EscribirFichero(tr[1], strToPointer(tr[2]), cont,0)) == -1)	//last  parameter=0 for not overwriting
    	    perror("There was an error");
    	else printf ("%s written bytes in %s from %p\n",tr[3],tr[1],strToPointer(tr[2]));
    }
    
}



void showEnvironment(char **entorno, char * nombre_entorno){
 int i=0;

 while (entorno[i]!=NULL) {
    printf ("%p->%s[%d]=(%p) %s\n", &entorno[i],
      nombre_entorno, i,entorno[i],entorno[i]);
    i++;
  }
}


int BuscarVariable(char * var, char *e[]){
  int pos=0;
  char aux[MAX];
  
  strcpy (aux,var);
  strcat (aux,"=");
  
  while (e[pos]!=NULL)
    if (!strncmp(e[pos],aux,strlen(aux)))
      return (pos);
    else 
      pos++;
  errno=ENOENT;   /*no hay tal variable*/
  return(-1);
}


int CambiarVariable(char * var, char * valor, char *e[]) 
{                                                        /*lo hace directamente, no usa putenv*/
  int pos;
  char *aux;
   
  if ((pos=BuscarVariable(var,e))==-1)
    return(-1);
 
  if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
	return -1;
  strcpy(aux,var);
  strcat(aux,"=");
  strcat(aux,valor);
  e[pos]=aux;
  return (pos);
}


char * Ejecutable(char *s){

	char path[MAX];
	static char aux2[MAX];
	struct stat st;
	char *p;

	if (s==NULL || (p=getenv("PATH"))==NULL)
		return s;
	if (s[0]=='/' || !strncmp (s,"./",2) || !strncmp (s,"../",3))
        return s;       /*is an absolute pathname*/
	strncpy (path, p, MAX);
	for (p=strtok(path,":"); p!=NULL; p=strtok(NULL,":")){
       sprintf (aux2,"%s/%s",p,s);
	   if (lstat(aux2,&st)!=-1)
		return aux2;
	}
	return s;
}


int OurExecvpe(char *file, char *const argv[], char *const envp[]){

   return (execve(Ejecutable(file),argv, envp));
}

char * getVariables(char *tr){
    char *aux2;
    char *new= malloc(MAX);

    new=strdup(tr);
    strcat(new,"=");  
    aux2=getenv(tr); 
    strcat(new,aux2); 
          
    return new;
}

int getLastArgument(char *tr[], int i){
    int k=i;

    for(int j=i; tr[j] != NULL; j++){
    	k=j;
    	if(tr[j][0] == '@' || !strcmp(tr[j],"&")){
    	    k--;
    	    break;
    	}
    }
    return k;
}





void executeThis(bool thereAreVariables, char *variables[MAX],char *tr, char*arguments[MAX]){
    if(thereAreVariables){
     	if((OurExecvpe(tr,arguments,variables)) == -1)
    	     perror("There was an error");
    }else{
     	if((OurExecvpe(tr,arguments,env1)) == -1)
    	     perror("There was an error");
    }
}

bool isThatCharacter(char *tr[],int lastArgument ,char character){
    
    if(tr[lastArgument+1] == 0)
    	return false;
    	
    if(tr[lastArgument+1][0] == '@')
        return true;
    else return false;
}

char * userName(uid_t uid){
    struct passwd *p;
    if ((p=getpwuid(uid))==NULL)
        return (" ???");
    return p->pw_name;
}

struct SEN{
    char *nombre;
    int senal;
};


static struct SEN sigstrnum[]={   
	{"HUP", SIGHUP},
	{"INT", SIGINT},
	{"QUIT", SIGQUIT},
	{"ILL", SIGILL}, 
	{"TRAP", SIGTRAP},
	{"ABRT", SIGABRT},
	{"IOT", SIGIOT},
	{"BUS", SIGBUS},
	{"FPE", SIGFPE},
	{"KILL", SIGKILL},
	{"USR1", SIGUSR1},
	{"SEGV", SIGSEGV},
	{"USR2", SIGUSR2}, 
	{"PIPE", SIGPIPE},
	{"ALRM", SIGALRM},
	{"TERM", SIGTERM},
	{"CHLD", SIGCHLD},
	{"CONT", SIGCONT},
	{"STOP", SIGSTOP},
	{"TSTP", SIGTSTP}, 
	{"TTIN", SIGTTIN},
	{"TTOU", SIGTTOU},
	{"URG", SIGURG},
	{"XCPU", SIGXCPU},
	{"XFSZ", SIGXFSZ},
	{"VTALRM", SIGVTALRM},
	{"PROF", SIGPROF},
	{"WINCH", SIGWINCH}, 
	{"IO", SIGIO},
	{"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
	{"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
	{"PWR", SIGPWR},
#endif
#ifdef SIGEMT
	{"EMT", SIGEMT},
#endif
#ifdef SIGINFO
	{"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
	{"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
	{"CLD", SIGCLD},
#endif
#ifdef SIGLOST
	{"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
	{"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
	{"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
	{"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
	{"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
	{"WAITING", SIGWAITING},
#endif
 	{NULL,-1},
	};    /*fin array sigstrnum */

char *NombreSenal(int sen)  /*devuelve el nombre senal a partir de la senal*/ 
{			/* para sitios donde no hay sig2str*/
 int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	if (sen==sigstrnum[i].senal)
		return sigstrnum[i].nombre;
 return ("SIGUNKNOWN");
}


char * getTime(char buffer[MAX]){
    
    time_t thisDay;
    int day,hours,month,minutes, year, seconds;
    
    time(&thisDay);
    
    struct tm *local = localtime(&thisDay);
    day = local -> tm_mday;
    hours = local -> tm_hour;
    month = local -> tm_mon + 1;
    minutes = local -> tm_min;
    year = local -> tm_year + 1900;
    seconds = local -> tm_sec;
    
    sprintf(buffer, "%d/%d/%d  %02d:%02d:%02d", year, month, day, hours, minutes, seconds);
    
    return buffer;
       
}






