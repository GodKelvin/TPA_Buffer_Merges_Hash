#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int size = 1000;
    int *intArray = malloc(sizeof(int) * size);
    for(int i = 0; i < size; i++)
    {
        intArray[i] = i;
    }

    srand(time(NULL));
    int randNum = rand() % size;

    printf("intArray[%d]: %d\n", randNum, intArray[randNum]);

    //Encerando sem realizar o free();
    free(intArray);
    return 0;
}