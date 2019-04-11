#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char *args[]={"ls","yay",NULL};
    execvp(args[0],args);
    printf("Ending-----\n"); //should not get here because execvp leads to the ending of this program and onto the called one
    return 0;
}
