# soal-shift-sisop-modul-3-B11-2021

Anggota kelompok :
* 05111940000181 - Cliffton Delias Perangin Angin
* 05111940000095 - Fuad Elhasan Irfani
* 05111940000107 - Sabrina Lydia Simanjuntak

## Soal 1

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
