#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
    int pipe1[2];             // sort <===> head
    int pipe2[2];            //  ps  <===> sort

    pid_t fork1;
    pid_t fork2;

    pipe(pipe1);
    fork1 = fork();
    if (fork1 == 0){ // child fork1
        pipe(pipe2);
        fork2 = fork();
        if (fork2 == 0) { // child fork2
            dup2(pipe2[1], STDOUT_FILENO);
            close(pipe2[0]);
            close(pipe2[1]);
            execlp("ps", "ps", "aux", NULL);
        }
        else { // parent fork2
            dup2(pipe2[0], STDIN_FILENO);
            dup2(pipe1[1], STDOUT_FILENO);
            close(pipe2[0]);
            close(pipe2[1]);
            execlp("sort", "sort", "-nrk", "3,3", NULL);
        }
    }
    else { // parent fork1
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);
        execlp("head", "head", "-5", NULL);
    }
}
