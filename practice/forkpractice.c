#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    // Two pipes:
    // First pipe to send input string from parent
    // Second pipe to send concatenated string from child
    int fd1[2]; // used to store two ends of first pipe
    int fd2[2]; //used to stores two ends of second pipe

    char fixed[] = "hello world";
    char input_str[100];
    pid_t p;

    if (pipe(fd1) == -1) {printf("error1\n");return 1;}
    if (pipe(fd2) == -1) {printf("error2\n");return 1;}
    scanf("%s", input_str);
    if ((p = fork()) < 0) {
        printf("fork fail\n");
        return 1;
    } else if (p > 0) { //parent process
        char concat[100];
        //close reading end of first pipe
        close(fd1[0]);

        //write input string and close writing end of first pipe
        write(fd1[1], input_str, strlen(input_str)+1);
        close(fd1[1]);

        //wait for child to send a string
        wait(NULL);

        //close writing end of second pipe
        close(fd2[1]);

        //read string from child, print it, and close reading end
        read(fd2[0], concat, 100);
        printf("concatted string: %s\n", concat);
        close(fd2[0]);
    } else { //child process
        //close writing end of first pipe
        close(fd1[1]);

        //read a string using first pipe
        char concat[100];
        read(fd1[0], concat, 100);

        //concatenate a fixed string with it
        int k = strlen(concat);
        int i;
        for (i = 0; i < strlen(fixed); i++) {
            concat[k++] = fixed[i];
        }

        //string ends with '\0'
        concat[k] = '\0';

        //close both reading ends
        close(fd1[0]);
        close(fd2[0]);

        //write concated string and close writing end
        write(fd2[1], concat, strlen(concat)+1);
        close(fd2[1]);
        exit(0);
    }
}
