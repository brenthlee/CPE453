#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MSGSIZE 14

/* simple pipes */
/*
int main() {
    char inbuf[MSGSIZE];
    int p[2], i;
    char *msg1 = "hello world 1";
    char *msg2 = "hello world 2";
    char *msg3 = "hello world 3";
    if (pipe(p) < 0) {
        exit(1);
    }
    write(p[1], msg1, MSGSIZE);
    write(p[1], msg2, MSGSIZE);
    write(p[1], msg3, MSGSIZE);
    for (i = 0; i < 3; i++) {
        read(p[0], inbuf, MSGSIZE);
        printf("%s\n", inbuf);
    }
    return 0;
}
*/

/* pipe with fork */
int main() {
    char inbuf[MSGSIZE];
    int p[2], pid, nbytes;
    char *msg1 = "hello world 1";
    char *msg2 = "hello world 2";
    char *msg3 = "hello world 3";
    if (pipe(p) < 0) {
        exit(1);
    }
    if ((pid = fork()) > 0) {
        write(p[1], msg1, MSGSIZE);
        write(p[1], msg2, MSGSIZE);
        write(p[1], msg3, MSGSIZE);
        close(p[1]); //adding this line will not hang the program (BLOCK)
        wait(NULL); //this will hang
    } else {
        close (p[1]); //adding this line will not hang the program
        while ((nbytes = read(p[0], inbuf, MSGSIZE)) > 0) {
            printf("%s\n", inbuf);
        }
        if (nbytes != 0) {
            exit(2);
        }
        printf("Finished reading\n");
    }
    return 0;
}
