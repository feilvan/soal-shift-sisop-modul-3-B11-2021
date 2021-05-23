# soal-shift-sisop-modul-3-B11-2021

Anggota kelompok :
* 05111940000181 - Cliffton Delias Perangin Angin
* 05111940000095 - Fuad Elhasan Irfani
* 05111940000107 - Sabrina Lydia Simanjuntak

## Soal 1
### 1.a
#### Server dapat menerima lebih dari 1 koneksi Client sekaligus, namun hanya 1 Client yang dapat menggunakan fungsi-fungsi program. Client ke-2 dan seterusnya harus menunggu sampai Client pertama log out.

Client diminta id dan password saat register -> disimpan di file akun.txt

```c
id:password
id2:password2
```
Client diminta id dan password saat login -> dicocokkan dengan id:password yang ada di file akun.txt
#### Penjelasan
##### Konesi Client-Server
Pakai template koneksi socket Client dan Server yang ada di modul 3.
Pakai thread untuk dapat memungkinkan terjadinya >1 koneksi Client ke Server.
```c
while(true) {
  if ((new_socket = accept(server_fd, 
      (struct sockaddr *) &address, (socklen_t*) &addr_len)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
  }
  pthread_create(&(tid[total]), NULL, &client, &new_socket);
  total++;
}
```
Simpan jumlah Client yang sedang terkoneksi pada suatu variabel.

Tentukan apakah Client akan di terima atau di tahan aksesnya ke fungsi program berdasarkan jumlah koneksi.
```c
if (total == 1) {
  send(new_socket, hello, STR_SIZE, 0);
}
else {
    send(new_socket, deny, STR_SIZE, 0);
}
```
Di Client, akan diperiksa pesan yang diterima dari Server.
```c
while (!equal(hello, buffer))
```
jika Server tidak mengirim pesan yang menandakan Client dapat menggunakan program, tunggu di while loop.
Kembali di sisi Server, selain meng-handle Client pertama, buat Client tersebut untuk menunggu.
```c
while (total > 1) {
  valread = read(new_socket, buffer, STR_SIZE);
  if (total == 1) {
      send(new_socket, hello, STR_SIZE, 0);
  }
  else {
      send(new_socket, deny, STR_SIZE, 0);
  }
}
```
![client wait](https://user-images.githubusercontent.com/73324192/119247216-f93c0d80-bbb1-11eb-88f8-223318aa2310.png)

Ketika meng-handle Client pertama, siapkan Server untuk menunggu command dari Client. Seperti: login, register, dst.
![client connected](https://user-images.githubusercontent.com/73324192/119247240-1bce2680-bbb2-11eb-8d87-d5b9cbf6daf9.png)

Ketika Client memutus koneksinya, decrement jumlah koneksi
```c
else if (equal(quit, buffer)) {
  close(new_socket);
  total--;
  break;
}
```
Terima Client yang sedang menunggu apabila menjadi koneksi pertama
![client finally connected](https://user-images.githubusercontent.com/73324192/119247255-2dafc980-bbb2-11eb-8245-c3330c46392a.png)

##### Fungsi register
Client mengirimkan command register ke Server.
Server menerima command dan menyiapkan file akun.txt.
```c
fp2 = fopen("akun.txt", "a");
```
Client-Server kirim-terima username dan password yang ingin di-register

![register](https://user-images.githubusercontent.com/73324192/119247272-46b87a80-bbb2-11eb-9e86-55ab894e5c95.png)

fprintf-kan user creds yang baru ke file akun.txt.
Close file dan akun telah diregister.
##### Fungsi login
Client mengirimkan command login ke Server.
Server menerima command.
Client-Server kirim-terima username dan password.
![login (2)](https://user-images.githubusercontent.com/73324192/119247291-68b1fd00-bbb2-11eb-8618-df3a5cb4cbb3.png)

Server membuka file akun.txt.
Baca file line-by-line dan cari login credentials yang sesuai.
```c
while ((file_read = getline(&line, &len, fp3) != -1))
```
Kirim pesan ke Client apakah login berhasil atau tidak.
Jika login berhasil, siapkan Client-Server untuk command add, download, dst.
![login](https://user-images.githubusercontent.com/73324192/119247295-723b6500-bbb2-11eb-8630-0d981f638f60.png)

### 1.b
#### Di Server terdapat files.tsv yang menyimpan path file di server, publisher, tahun publikasi. files.tsv di-update setiap ada operasi add dan delete files. Folder FILES yang menyimpan semua file yang dikirimkan oleh Client, otomatis dibuat saat server dijalankan.

#### Penjelasan
##### files.tsv
Pada operasi penambahan file, tambahkan kode yang akan membuat file files.tsv.
Perubahan pada file tersebut akan dijelaskan lebih lanjut pada bagian soal selanjutnya.
##### folder FILES
Buat folder FILES setelah Server dijalankan.

### 1.c
#### Membuat fitur agar client dapat menambah file baru ke dalam server
#### Penjelasan
##### Command add dari Client
Client menerima command dan meneruskannya ke Server.
Server bersiap untuk menerima detail dari file.
Client mengirimkan detail dari file.

![add](https://user-images.githubusercontent.com/73324192/119247303-82534480-bbb2-11eb-9604-8fcf64bbc91b.png)

##### Pengiriman file ke Server
Client membuka file dan mengirimkan file ke Server
```c
int fd = open(data, O_RDONLY);
if (!fd) {
  perror("can't open");
  exit(EXIT_FAILURE);
}

int read_len;
while (true) {
  memset(data, 0x00, STR_SIZE);
  read_len = read(fd, data, STR_SIZE);

  if (read_len == 0) {
    break;
  }
  else {
    send(sock, data, read_len, 0);                               
  }
}
close(fd);
```
Server menerima file dan menyimpannya
```c
int des_fd = open(request.path, O_WRONLY | O_CREAT | O_EXCL, 0700);
if (!des_fd) {
  perror("can't open file");
  exit(EXIT_FAILURE);
}
```
![struktur files](https://user-images.githubusercontent.com/73324192/119247320-9b5bf580-bbb2-11eb-852f-f95b046c8259.png)

##### Penambahan detail file di files.tsv
Server menerima detail file terlebih dahulu
```c
valread = read(new_socket, request.publisher, STR_SIZE);
valread = read(new_socket, request.year, STR_SIZE);
valread = read(new_socket, clientPath, STR_SIZE);
```
Server memproses file path dari Client dan memodifikasinya agar dapat menyimpan file di FILES/nama_file.extension.
Server membuka files.tsv dan menambahkan detail file.
```c
fp = fopen("files.tsv", "a");
fprintf(fp, "%s\t%s\t%s\n", request.path, request.publisher, request.year);
fclose(fp);
```
![files tsv](https://user-images.githubusercontent.com/73324192/119247324-a9117b00-bbb2-11eb-9459-0cd21f242b7f.png)

### 1.d
#### Membuat fitur agar client dapat mendownload file yang ada di Server -> check di files.tsv
#### Penjelasan
##### Download file
Solusi dari download sangat serupa dengan solusi dari add, hanya saja peran Client-Server dibalik.
Client mengirimkan command download nama_file.extension.
Server menerima command dan mencari nama file di files.tsv.

![download](https://user-images.githubusercontent.com/73324192/119247400-42d92800-bbb3-11eb-9f4d-f7d61c630392.png)

![download not found](https://user-images.githubusercontent.com/73324192/119247406-48367280-bbb3-11eb-9620-616cf740f94e.png)

Server akan mengirimkan file ke Client jika ada file yang diminta.
### 1.e
#### Client mengirimkan command delete nama_file.extension.Jika file ada, file hanya di-rename menjadi old-nama_file.extension. Hapus file dari list pada files.tsv
#### Penjelasan
##### Client send command
Client mengirimkan command delete dan nama file yang ingin di-delete

Server menerima command dan mengecek apakah ada file tersebut di files.tsv

![delete](https://user-images.githubusercontent.com/73324192/119247415-6b612200-bbb3-11eb-8061-fa1bc1f57e61.png)

![delete not found](https://user-images.githubusercontent.com/73324192/119247417-7025d600-bbb3-11eb-89ee-8e928ed243fb.png)

##### Rename file
jika ada file yang ingin dihapus, rename nama file
```c
found = true;
char old[] = "FILES/old-";
strcat(old, temp_entry.name);
rename(temp_entry.path, old);
```
##### Ubah files.tsv
Selama membaca files.tsv, simpan pada baris ke-berapa terdapat detail dari file yang dihapus
Buat fungsi untuk menghapus baris tertentu di files.tsv
```c
removeLine(index);
```
fungsi merupakan modifikasi dari fungsi yang ditemukan di-web. https://www.w3resource.com/c-programming-exercises/file-handling/c-file-handling-exercise-8.php
### 1.f
#### Client dapat melihat semua isi files.tsv dengan memanggil suatu perintah yang bernama see
#### Penjelasan
##### Client mengirimkan command see
Client-Server kirim-terima command
Server membuka files.tsv
##### Output di client
Server membaca files.tsv line-by-line
Server memproses tiap line agar mendapatkan informasi yang diperlukan
Selama masih ada baris untuk diproses, kirim informasi ke Client

```c
char message[STR_SIZE];
sprintf(message, "Nama : %s\nPublisher : %s\nTahun Publishing : %s\nEkstensi File : %s\nFilepath : %s\n\n", 
  temp_entry.name, temp_entry.publisher, temp_entry.year, ext, temp_entry.path);
                          
send(new_socket, message, STR_SIZE, 0);
```
Client menerima informasi dari Server selama masih ada informasi untuk diterima

![see](https://user-images.githubusercontent.com/73324192/119247453-c1ce6080-bbb3-11eb-9cdd-3bdaaa61ccd3.png)

### 1.g
#### Client menerima detail files yang mengandung string yang dikirimkan
#### Penjelasan
##### Client send command
Client-Server kirim-terima
Server membaca files.tsv line-by-line
Manfaatkan strstr agar sesuai dengan apa yang diminta soal
```c
if ((h = strstr(temp_entry.name, buffer)) != NULL)
```
### 1.h
#### Tiap operasi add dan delete, log di running.log
#### Penjelasan
##### log tiap add dan delete
Buat fungsi untuk memudahkan penambahan log di running.log
```c
void log_action(char *type, char *fileName, char *user, char *pass) {
    FILE *log;
    char action[16];

    if (equal(type, "add")) {
        strcpy(action, "Tambah");
    }
    else if (equal(type, "delete")) {
        strcpy(action, "Hapus");
    }

    log = fopen("running.log", "a");
    fprintf(log, "%s : %s (%s:%s)\n", action, fileName, user, pass);
    fclose(log);

    return;
}
```
Fungsi menerima apa tipe operasinya (add/delete), file apa yang terpengaruh, siapa yang melakukannya beserta passwordnya
ketika Server memproses add dan delete, panggil fungsi tersebut
```c
log_action("add", request.name, akun.name, akun.password);
```
Panggil fungsi ketika terjadi file addition
```c
log_action("delete", temp_entry.name, akun.name, akun.password);
```
Panggil fungsi ketika terjadi deletion

![running log](https://user-images.githubusercontent.com/73324192/119247470-f3dfc280-bbb3-11eb-9545-9fad20f956e5.png)

### Kendala
Materi dirasa susah dan karena minggu praktikum berbarengan dengan minggu ets

## Soal 2
### 2.a
#### Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).
Pertama buat matriksnya. "r" adalah row (baris). "c" adalah column (kolom). "m" adalah array untuk matriks.
```c
    int r1 = 4, c1 = 3;
    int r2 = 3, c2 = 6;
    int m1[r1][c1];
    int m2[r2][c2];
```
Lalu input angka ke matriks 1 dan 2.
```c
    for (int i=0; i<r1; i++) {
        for (int j=0; j<c1; j++){
            scanf("%d", &m1[i][j]);
        }
    }
    printf("\n");
    for (int i=0; i<r2; i++) {
        for (int j=0; j<c2; j++){
            scanf("%d", &m2[i][j]);
        }
    }
```
Buat lagi matriks untuk menyimpan hasil perkalian kedua matriks. Ukuran matriks hasil perkalian adalah row 1 (r1) x column 2 (c2).
```c
    int r3 = r1, c3 = c2;
    int m3[r3][c3];
```
Lakukan perkalian matriks lalu simpan hasilnya di matriks 3 (m3)
```c
    for (int i=0; i<r3; i++) {
        for (int j=0; j<c3; j++) {
            m3[i][j]=0;
            for (int k=0; k<c1; k++) {
                m3[i][j] += ((m1[i][k]) * (m2[k][j]));
            }
        }
    }
```
Tampilkan hasil dari perkalian matriks
```c
    printf("\n");
    for (int i=0; i<r3; i++) {
        for (int j=0; j<c3; j++) {
            printf("%d  ", m3[i][j]);
        }
        printf("\n");
    }
```
Agar matriks bisa dipakai di soal 2b maka hasil perkalian matriks dan ukurannya dimasukkan ke shared memory
```c
    int segid1=shmget(1234, sizeof(int), IPC_CREAT | 0666);
    int segid2=shmget(1235, sizeof(int), IPC_CREAT | 0666);
    int segid3=shmget(1236, sizeof(int), IPC_CREAT | 0666);
    int *matrix, *row, *column;

    matrix = (int *)shmat(segid1,NULL,0);
    row = (int *)shmat(segid2,NULL,0);
    column = (int *)shmat(segid3,NULL,0);

    *row = r3;
    *column = c3;

    for(int i=0;i<r1;++i){
        for(int j=0;j<c2;++j){
            matrix[i*c2+j] = m3[i][j];
        }
    }
    
    sleep(30);

    shmdt(matrix);
    shmdt(row);
    shmdt(column);
    shmctl(segid1,IPC_RMID,NULL);
    shmctl(segid2,IPC_RMID,NULL);
    shmctl(segid3,IPC_RMID,NULL);
    return 0;
```
Berikut screenshot saat program dijalankan
![a](https://raw.githubusercontent.com/feilvan/sisop-general/main/modul3/ss/2a.png)

#### Kendala :
Tidak bisa memasukkan array dua dimensi ke shared memory. Jadi saya akali dengan mengubahnya menjadi satu dimensi. 

### 2.b
#### Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) (Catatan!: gunakan shared memory). Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil) (Catatan!: gunakan thread untuk perhitungan di setiap cel).
Sebelum masuk ke fungsi main, saya buat beberapa variabel global untuk mempermudah pengerjaan.
</br>Yang pertama ada struct. Karena pada saat membuat thread hanya bisa menampung satu argument maka saya menggunakan struct agar bisa memasukkan lebih dari satu argument
```c
struct arg_struct {
    int x, a, b;
};
```
Lalu ada variabel global matrixfinal. Matriks ini digunakan untuk menyimpan hasil perhitungannya. Saya jadikan variabel global agar bisa diakses fungsi faktorial.
```c
unsigned long long matrixfinal[26];
```
Selanjutnya masuk ke fungsi main. Pertama ambil row, column, dan matriks dari shared memory.
```c
    int segid1=shmget(1234, sizeof(int), IPC_CREAT | 0666);
    int segid2=shmget(1235, sizeof(int), IPC_CREAT | 0666);
    int segid3=shmget(1236, sizeof(int), IPC_CREAT | 0666);
    int *matrix, *row, *column;
    
    matrix = (int *)shmat(segid1,NULL,0);
    row = (int *)shmat(segid2,NULL,0);
    column = (int *)shmat(segid3,NULL,0);

    int *r = row;
    int *c = column;

    for(int i=0;i<(*r);++i)
        for(int j=0;j<(*c);++j)
            matrixfinal[i*(*c)+j] = matrix[i*(*c)+j];
```
Buat matriks baru lalu input dengan batas maksimal faktorial.
```c
    int mbaru[(*r)*(*c)];
    for(int i=0;i<(*r);++i){
        for(int j=0;j<(*c);++j){
            //printf("MBaru %d %d: ", i+1, j+1);
            scanf("%d", &mbaru[i*(*c)+j]);
        }
        //printf("\n");
    }
```
Karena di soal diminta untuk melakukan perhitungan menggunakan thread, maka saya buat thread sebanyak besar matriksnya, lalu menjalankan fungsi faktorial. Agar thread dipastikan selesai sebelum program ditutup maka semua thread dilakukan thread join.
```c
    pthread_t tid[(*r)*(*c)];
    struct arg_struct args[(*r)*(*c)];

    for (int i=0; i<(*r); i++)
        for (int j=0; j<(*c); j++) {
            args[i*(*c)+j].x = i*(*c)+j;
            args[i*(*c)+j].a = matrixfinal[i*(*c)+j];
            args[i*(*c)+j].b = mbaru[i*(*c)+j];
            pthread_create(&tid[i*(*c)+j],NULL,&faktorial,(void*)&args[i*(*c)+j]);
        }

    for (int i=0; i<(*r); i++)
        for (int j=0; j<(*c); j++)
            pthread_join(tid[i*(*c)+j], NULL);
```
Berikut code fungsi faktorialnya. Jika b = 0 maka hasilnya sama dengan 0. Jika b = 1 biarkan saja karena hasilnya sama dengan angka awalnya. Jika b > 1 maka lakukan faktorial. Disini saya menggunakan for loop untuk menghitungnya. Loop akan berjalan sebanyak tidak lebih dari a dan b. Jadi angkanya akan dikalikan sebanyak a atau b tergantung mana yang paling sedikit antara a dan b.
```c
void *faktorial (void *arg){
    struct arg_struct *args = arg;
    if (args->b==0)
        matrixfinal[args->x]=0;
    else if (args->b > 1)
        for (int i=1; i<args->b && i<args->a; i++)
            matrixfinal[args->x] = matrixfinal[args->x] * (args->a-i);
}
```
Tampilkan hasilnya.
```c
    printf("\n");
    for (int i=0; i<*row; i++) {
        for (int j=0; j<*column; j++)
            printf("%llu  ", matrixfinal[i*(*column)+j]);
        printf("\n");
    }
```
Berikut screenshot saat program dijalankan
![b](https://raw.githubusercontent.com/feilvan/sisop-general/main/modul3/ss/2b.png)

#### Kendala :
Tidak ada.

### 2.c
#### Karena takut lag dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5” (Catatan!: Harus menggunakan IPC Pipes).
Buat variabel untuk pipe dan fork. Ada dua pipe dan dua fork. Pertama untuk menjalankan pipe antara sort dan head, keduan untuk menjalankan pipe antara ps dan sort.
```c
    int pipe1[2];             // sort <===> head
    int pipe2[2];            //  ps  <===> sort

    pid_t fork1;
    pid_t fork2;
```
Buat fork dan pipe yang pertama.
```c
    pipe(pipe1);
    fork1 = fork();
```
Di parent processnya dilakukan pipe antara output dari child processnya dengan head.
```c
    else { // parent fork1
        dup2(pipe1[0], STDIN_FILENO); // duplicate in pipe1 --> stdin
        close(pipe1[0]); // close in pipe1
        close(pipe1[1]); // close out pipe1
        execlp("head", "head", "-5", NULL); // head --> stdin
    }
```
Di child processnya dilakukan fork dan pipe yang kedua.
```c
    if (fork1 == 0){ // child fork1
        pipe(pipe2);
        fork2 = fork();
```
Di dalam fork yang kedua dilakukan pipe antara ps dan aux.
```c
        if (fork2 == 0) { // child fork2
            dup2(pipe2[1], STDOUT_FILENO); // duplicate out pipe2 --> stdout
            close(pipe2[0]); // close in pipe2
            close(pipe2[1]); // close out pipe2
            execlp("ps", "ps", "aux", NULL); // ps --> stdout
        }
        else { // parent fork2
            dup2(pipe2[0], STDIN_FILENO); // duplicate in pipe2 --> stdin
            dup2(pipe1[1], STDOUT_FILENO); // duplicate out pipe1 --> stdout
            close(pipe2[0]); // close in pipe2
            close(pipe2[1]); // close out pipe2
            execlp("sort", "sort", "-nrk", "3,3", NULL); // sort --> stdin & stdout
        }
    }
```
Berikut screenshot saat program dijalankan. Terbukti outputnya sama dengan memasukkan commandnya di terminal.
![c](https://raw.githubusercontent.com/feilvan/sisop-general/main/modul3/ss/2c.png)

#### Kendala :
Kurang paham dengan cara kerja pipes. Jadi saya mencari banyak referensi dan melakukan cukup banyak trial & error.

## Soal 3
 Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan sesuai dengan ekstensinya. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan. Apabila file tersebut tidak memiliki ekstensi maka file tersebut akan dipindahkan ke folder Unknown. Terdapat tiga mode untuk mengkategorikan file, yaitu -f, *, dan -d.

### 3.a
 Pada soal 3.a, apabila argumennya adalah "-f", maka program akan mengkategorikan file yang ada pada argumen ke dalam folder berdasarkan ekstensi file tersebut, dan folder berada pada working directory program.
```c
if (strcmp(argv[1],"-f")==0){  
        perintah = argv[1];
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
  ```
  Di sini program mengecek terlebih dahulu argv[1], apabila merupakan '-f' maka kode tersebut akan melakukan looping sebanyak argumen file yang diinput dan memasukkannya ke thread yang dibuat untuk memindahkan file. Di sini juga digunakan pthread_join untuk memastikan program telah selesai tereksekusi baru melanjut ke yang berikutnya.

### 3.b
 Pada 3.b, apabila argumen yang diterima adalah "-d", maka program akan mengkategorikan semua file pada folder yang diberikan pada argumen ke dalam folder berdasarkan ekstensi file tersebut, dan folder berada pada working directory program.
```c
else if (strcmp(argv[1],"-d") == 0 ) { 
        perintah = argv[1];
        recursive(argv[2]);
        for(int i=0; i<nomor; i++){
            pthread_create(&(thread[i]), NULL, myFile, (char*)namaFilesNih[i]);
        }
        for(int i=0; i<nomor; i++){
            pthread_join(thread[i],NULL);
        }
        if(keberhasilan == 1){
            printf("Direktori sukses disimpan!\n");
        }else{
            printf("Yah, gagal disimpan :(\n");
        }
    } 
 ```
  Di sini, program mengecek terlebih dahulu argv[1], apabila merupakan '-d' maka kode tersebut akan mengerjakan kode tersebut dan memasukkannya ke thread yang dibuat.  Di sini juga digunakan pthread_join untuk memastikan program telah selesai tereksekusi baru melanjut ke yang berikutnya. Lalu, apabila direktori sukses ataupun gagal, program akan memberi output pemberitahuan.

### 3.c
 Pada soal 3.c, apabila argumen yang diterima adalah "*" program akan mengkategorikan semua file yang ada pada working directory ke dalam folder berdasarkan eksistensinya.
```c 
else if (strcmp(argv[1],"*") == 0 ) { 
       perintah = argv[1];
       recursive(".");
       for(int i=0; i<nomor; i++){
           pthread_create(&(thread[i]), NULL, myFile, (char*)namaFilesNih[i]);
       }
       for(int i=0; i<nomor; i++){
           pthread_join(thread[i],NULL);
       }
       if(keberhasilan == 1){
           printf("Direktori sukses disimpan!\n");
       }else{
           printf("Yah, gagal disimpan :(\n");
       }
   }
   
   return 0;
}
```
 Di sini, program mengecek terlebih dahulu argv[1], apabila merupakan '*' maka program tersebut akan mengerjakan kode tersebut dan memasukkannya ke thread yang dibuat.  Di sini juga digunakan pthread_join untuk memastikan program telah selesai tereksekusi baru melanjut ke yang berikutnya. Lalu, apabila direktori sukses ataupun gagal, program akan memberi output pemberitahuan.

Untuk melakukan traverse directory, maka dibuatkan fungsi recursive berikut.
```c 
 void recursive(char *basePath) //traverse direktori
{
 char path[1000];
 struct dirent *dp;
 DIR *dir = opendir(basePath);

 if (!dir)
     return;

 while ((dp = readdir(dir)) != NULL)
 {
     if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
     {
         
         strcpy(path, basePath);
         strcat(path, "/");
         strcat(path, dp->d_name);
         strcpy(namaFilesNih[nomor], path);
         nomor++;
         recursive(path);
     }
 }

 closedir(dir);
}
```
 Di sini, dibuat pointer untuk menunjuk ke directory, lalu dilakukan looping untuk directory yang mana dalam directory tersebut ada file/folder didalamnya.
 
### 3.d dan 3.e
 Pada soal ini, semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “unknown”. Jika file hidden, masuk folder “hidden”. Dimana, setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.
```c 
 if(namaFiles[0]=='.'){
       ext = "hidden";
   }else{
       namaFiles = strtok(namaFiles, ".");
       if(strcmp(namaFilesLama, namaFiles)==0){
           ext = "unknown"; 
       }else{
           ext = strtok(NULL, "");
           for (int i=0;i < strlen(ext);i++){
               ext[i] = tolower(ext[i]);
           }
       }
  ```
 Pada bagian ini, dicek, apabila file berawalan '.' maka file tersebut terkategorikan hidden. Lalu, file juga dicek menggunakan strtok, apabila file tidak memiliki ekstensi, maka akan dikategorikan 'unknown'
 
 Berikut merupakan merupakan isi dari thread myFile yang mengkategorikan suatu file dan untuk mendapatkan nama file beserta ekstensinya. Sementara kode di bawah ini untuk mendapatkan ekstensi dari file tersebut.
 ```c 
    pthread_t thread[1000];
    void *myFile (void *judulFiles) {
    char *ext;
    char judulBaru[2000];
    char *judulFiles1 = (char*) judulFiles;
    snprintf(judulBaru, sizeof judulBaru, "%s", judulFiles1);
    char *judul = judulFiles;
    char *namaFiles;
    namaFiles = strrchr(judulFiles, '/');
    namaFiles = strtok(namaFiles, "/");
    char namaFilesLama[10000];
    snprintf(namaFilesLama, sizeof namaFilesLama, "%s", namaFiles);
 ```
 Pada soal juga diminta bahwa ekstensinya tidak case sensitive, maka digunakan kode berikut untuk mengubah apabila terdapat huruf kapital menjadi huruf kecil.
      ```c 
      for (int i=0;i < strlen(ext);i++){
                ext[i] = tolower(ext[i]);
            }
      ```
  #### Input "-f"
  ![image](https://user-images.githubusercontent.com/83162422/119263925-3da7c780-bc0b-11eb-9f7c-c719060e8ed8.png)
  
  #### Output "-f"
  ![image](https://user-images.githubusercontent.com/83162422/119263967-6d56cf80-bc0b-11eb-93e6-c07d9c681cc1.png)
![image](https://user-images.githubusercontent.com/83162422/119263982-7cd61880-bc0b-11eb-8991-0ffd46d3f63c.png)
 
 #### Output "-d"
 ![image](https://user-images.githubusercontent.com/83162422/119264073-d9393800-bc0b-11eb-93ea-d67c5cd3ed23.png)
 
 #### Output "*"
 ![image](https://user-images.githubusercontent.com/83162422/119264321-dc80f380-bc0c-11eb-9c75-856d5ed4ce3b.png)



  
 #### Kendala
 Kendala dalam mengerjakan soal ini adalah agak kebingungan saat file-file yang ada tidak masuk ke dalam folder. Lalu agak bingung dalam menentukan file hidden karena tidak ada contohnya dan bingung file seperti apa yang masuk ke kategori hidden. 
