#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/stat.h>

struct arg_struct {
    int x, a, b;
};
unsigned long long matrixfinal[26];

void *faktorial (void *arg);

int main(int argc,char *argv[])
{

    // Init shared memory --------------------------------------------
    int segid1=shmget(1234, sizeof(int), IPC_CREAT | 0666);
    int segid2=shmget(1235, sizeof(int), IPC_CREAT | 0666);
    int segid3=shmget(1236, sizeof(int), IPC_CREAT | 0666);
    int *matrix, *row, *column;
    
    matrix = (int *)shmat(segid1,NULL,0);
    row = (int *)shmat(segid2,NULL,0);
    column = (int *)shmat(segid3,NULL,0);

    // Copy dari shared memory --------------------------------------------
    int *r = row;
    int *c = column;

    for(int i=0;i<(*r);++i)
        for(int j=0;j<(*c);++j)
            matrixfinal[i*(*c)+j] = matrix[i*(*c)+j];

    // Cek row, column, dan matriks --------------------------------------------
    printf("row = %d\n", *row);
    printf("column = %d\n", *column);
    printf("matrix =\n");
    for (int i=0; i<*row; i++) {
        for (int j=0; j<*column; j++) {
            printf("%llu  ", matrixfinal[i*(*column)+j]);
        }
        printf("\n");
    }
    printf("\n");

    // Input --------------------------------------------
    int mbaru[(*r)*(*c)];
    for(int i=0;i<(*r);++i){
        for(int j=0;j<(*c);++j){
            //printf("MBaru %d %d: ", i+1, j+1);
            scanf("%d", &mbaru[i*(*c)+j]);
        }
        //printf("\n");
    }

    // Thread hitung --------------------------------------------
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

    // Print matrixfinal --------------------------------------------
    printf("\n");
    for (int i=0; i<*row; i++) {
        for (int j=0; j<*column; j++)
            printf("%llu  ", matrixfinal[i*(*column)+j]);
        printf("\n");
    }
    
    // Detach shared memory --------------------------------------------
    shmdt(matrix);
    shmdt(row);
    shmdt(column);
    shmctl(segid1,IPC_RMID,NULL);
    shmctl(segid2,IPC_RMID,NULL);
    shmctl(segid3,IPC_RMID,NULL);
    return 0;
}

void *faktorial (void *arg){
    struct arg_struct *args = arg;
    if (args->b==0)
        matrixfinal[args->x]=0;
    else if (args->b > 1)
        for (int i=1; i<args->b && i<args->a; i++)
            matrixfinal[args->x] = matrixfinal[args->x] * (args->a-i);
}

/* Input urut 1-20 --------------------------------------------
1 2 3
4 5 6
7 8 9
10 11 12

1 2 3 4 5 6
7 8 9 10 11 12
13 14 15 16 17 18

1 0 2 1 3 1
3 0 0 1 2 1
4 2 1 5 2 0
1 4 2 1 3 2
*/

/* Input 1 semua --------------------------------------------
1 1 1
1 1 1
1 1 1
1 1 1

1 1 1 1 1 1
1 1 1 1 1 1
1 1 1 1 1 1

1 0 2 1 3 1
3 0 0 1 2 1
4 2 1 5 2 0
1 4 2 1 3 2
*/
