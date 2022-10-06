#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> 

int main(void)
{
    // This is a test to see if I can't get fork to work. 
    int pid = fork();
    if (pid == 0)
    {
        printf("I am in the parent process!");
    }
    else
    {
        printf("I am in the child process!");
    }
    
    #pragma omp parallel for
    for (int i = 0; i < 10; i++)
    {
        sleep(1000);
    }
}
