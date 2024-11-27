//Victor Nathanael Badillo Aldama, group 6.1 
//Pablo Legide Vidal, group 6.1
#include "headers.h"

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
            
            if(commandNumberTrozos == commandNumber || commandNumberTrozos == item.index)
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
    
    	    if((pw=getpwuid(fs.st_uid)) == NULL) perror("There was an error ");
    	    if((gr=getgrgid(fs.st_gid)) == NULL) perror("There was an error ");
    	
    	    //-acc option
    	    if((pr->acc) == 1) strftime(buffer,MAX,"%Y/%m/%d-%H:%M",localtime(&fs.st_atime));
    	    else strftime(buffer,MAX,"%Y/%m/%d-%H:%M",localtime(&fs.st_mtime));
    	 	
    	     printf("%s   %ld (  %ld)   %s   %s",buffer,fs.st_nlink,fs.st_ino,pw->pw_name,gr->gr_name);
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


