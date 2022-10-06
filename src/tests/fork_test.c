#include<omp.h> 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void)
{
//    int pid = fork();
//    if (pid == 0)
//    {
//        printf("I am in the parent process!\n");
//    }
//    else
//    {
//        printf("I am in the child process!\n");
//    }
 
    #pragma omp parallel for num_threads(10)
    for (int i = 0; i < 10; i++)
    {
        printf("I am in process %i\n", i);
        sleep(1);
    }
}
