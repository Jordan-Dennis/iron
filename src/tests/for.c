#include<stdio.h>
#include<stdlib.h>


int main(void)
{
    for (int i = 0, j = 0; i < 10; i +=2, j++)
    {
        printf("i: %i\n", i);
        printf("j: %i\n", j);
    }
}
