#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

int main () {

    // Input --------------------------------------------
    int r1 = 4, c1 = 3;
    int r2 = 3, c2 = 6;
    int m1[r1][c1];
    int m2[r2][c2];

    for (int i=0; i<r1; i++) {
        for (int j=0; j<c1; j++){
            printf("M1 %d %d:", i+1, j+1);
            scanf("%d", &m1[i][j]);
        }
    }
    
    for (int i=0; i<r2; i++) {
        for (int j=0; j<c2; j++){
            printf("M2 %d %d:", i+1, j+1);
            scanf("%d", &m2[i][j]);
        }
    }

    // Multiply --------------------------------------------
    int r3 = r1, c3 = c2;
    int m3[r3][c3];

    for (int i=0; i<r3; i++) {
        for (int j=0; j<c3; j++) {
            m3[i][j]=0;
            for (int k=0; k<c1; k++) {
                m3[i][j] += ((m1[i][k]) * (m2[k][j]));
            }
        }
    }

    // Display --------------------------------------------
    for (int i=0; i<r3; i++) {
        for (int j=0; j<c3; j++) {
            printf("%d  ", m3[i][j]);
        }
        printf("\n");
    }

    // Shared Memory --------------------------------------------
    key_t key = 1234;
    int segid=shmget(key, sizeof(int), IPC_CREAT | 0666);
    int *matrix;
    matrix = (int *)shmat(segid,NULL,0);

    for(int i=0;i<r1;++i){
        for(int j=0;j<c2;++j){
            matrix[i*c2+j] = m3[i][j];
        }
    }
    
    sleep(10);

    shmdt(matrix);
    shmctl(segid,IPC_RMID,NULL);
    return 0;
}
