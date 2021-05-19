#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>

pthread_t thread[1000];
int many =1;
void *myFile (void *judulFiles) {
    char *ext;
    char judulBaru[2000];
    char *judulFiles1 = (char*) judulFiles;
    snprintf(judulBaru, sizeof judulBaru, "%s", judulFiles1);
    char *judul = judulFiles;
    ext = strtok(judul, ".");
    if(strcmp(ext, judulBaru)==0){
        ext = "unknown";
    }else{
        ext = strtok(NULL, "");
    }
     
     //buat folder berdasarkan ekstensinya
     
    char lokasiPindah[10000];
    const char ch = '/';
    char *ret;
    char *namaFile;
    ret = strrchr(judulBaru, ch);
    
    if (ret != NULL) {
        namaFile = strtok (ret,"/");
       // printf ("%s\n",namaFile);
    
    }
    if (namaFile[0]=='.'){
      ext="hidden";
    }
    mkdir(ext,0777);
    snprintf(lokasiPindah,sizeof lokasiPindah,"%s/%s",ext,namaFile);
    

    int hasil; 
    hasil = rename(judulBaru,lokasiPindah);
     if(hasil == 0) 
   {
      printf("File %d : Berhasil Dikategorikan\n",many);
   }
   else 
   {
      printf("File %d : Sad,gagal :(\n",many);
   }
   many++;
}


int main(int argc, char *argv[]) {
  
  /*Perintah -f */

  //letak argumen pada char 1 akan dibandingkan dengan -f, jika sama maka akan diproses
  if (strcmp(argv[1],"-f")==0){  
    int jumlah = 2;
    int jalankan = 2;
    while (jumlah < argc) {
      pthread_create(&(thread[jumlah]), NULL, myFile, (char*)argv[jumlah]);
      jumlah++;
    }
    while (jalankan < argc) {
      pthread_join(thread[jalankan],NULL);
      jalankan++;
    }
  }
  
  /*Perintah -d */
  else if (strcmp(argv[1],"-d") == 0 ) {       
  }
  
  return 0;
}
