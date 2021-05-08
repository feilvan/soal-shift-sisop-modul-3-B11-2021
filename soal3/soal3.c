#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>




char cwd[100];
pthread_t tid[10]; 
pid_t child;
char *arr[4],*arr2[20],arr3[100],abc[100];
int n = 0, m = 0;

void* moveandmake(void *arg)
{
    strcpy(abc,arg);
    char *token,*token1;

    token1 = strtok(abc, "/");
    while( token1 != NULL ) {
        arr2[m] = token1;
        m++;
        token1 = strtok(NULL, "/");
    }
    strcpy(arr3,arr2[m-1]);
    token = strtok(arr2[m-1], ".");
    while( token != NULL ) {
        arr[n] = token;
        n++;
        token = strtok(NULL, ".");
    }

    char file[100];
    strcpy(file,arr[n-1]);
    for(int i = 0; file[i]; i++){
        file[i] = tolower(file[i]); //case sensitive
    }
    if (strcmp(argv[1],"-f")==0){  
    int sum= 2, run = 2;
    while (sum < argc) {
      pthread_create(&(thread[jumlah]), NULL, myFile, (char*)argv[jumlah]);
      sum++;
    }
    while (jalankan < argc) {
      pthread_join(thread[jalankan],NULL);
    sum++;
    }
  
    DIR *folder, *folderopen;
    struct dirent *entry;
    char st[100];
    folder = opendir(cwd); 
    int available = 0; //flag
    if( n > 1 ){

        if(folder == NULL)
        {
            printf("error\n");
        }
        while( (entry=readdir(folder)) )
        {
            if(strcmp(entry->d_name,file) == 0 && entry->d_type == 4){
                available = 1;
                break;
            }
        }

        if(available == 0){
            strcpy(st,cwd);
            strcat(st,"/");
            strcat(st,file);
            mkdir(st, 0700);
        }
    }else{
        strcpy(st,cwd);
        strcat(st,"/");
        strcat(st,"Unknown");
        mkdir(st, 0700);
    }
 


 return NULL;
}


int main(int argc, char *argv[]) {
    int many =1
        
     if(getcwd(cwd, sizeof(cwd)) != NULL) 
   {
      printf("File %d : Berhasil Dikategorikan\n",many);
   }
   else 
   {
      printf("File %d : Sad,gagal :(\n",many);
   }
   many++;
    int i=0,j=0;
    if (strcmp(argv[1],"-f") == 0) {
        for(j = 2 ; j < argc ; j++ ){
            pthread_create(&(tid[i]),NULL,moveandmake,argv[j]);
            pthread_join(tid[i],NULL);
            i++;
        }
    }
    return 0; 
}
