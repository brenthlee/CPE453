#include <stdlib.h>
#include <stdio.h>

#define ARR_SIZE 10000

void main(int argc, char *argv[]) {
    int seed;
    int i;
    printf("Enter an int for the seed: ");
    scanf(" %d", &seed);
    FILE *f;
    f = fopen("numbers.txt", "w");
    srand(seed);
    int x;
    for (i = 0; i < ARR_SIZE; i++) {
        x = rand() % ARR_SIZE;
        fprintf(f, "%d ", x);
    }
    close(f);
}
