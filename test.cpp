#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MIN_PID 100
#define MAX_PID 1000

int size_of_thread;
int thread_number = 0;

pthread_mutex_t mutex;

struct pid_tab
{
    int pid;
    float bitmap;
}pidArr[1000];
int allocate_map(void)                                  //allocates bitmap values to the data structure
{
    int i,j;
    for(i = MIN_PID, j =0; i <= MAX_PID; i++, j++)
    {
        pidArr[j].pid = i;
        pidArr[j].bitmap = 0;
    }
    if(i == MAX_PID && j == MAX_PID)
    {
    return 1;
	}
    else
    {
	return -1;
	}
}
int allocate_pid(void)                                  //allocates a pid to the new process
{
	int i,j;
    for(i = MIN_PID, j =0; i <= MAX_PID; i++, j++)
    {
        if(pidArr[j].bitmap == 0)
        {
            pidArr[j].pid = i;
            pidArr[j].bitmap = 1;
            return i;
            break;
        }
    }
}
void release_pid(int rpid)                               //releases pid
{
	int i;
    for(i = 0; i <= MAX_PID; i++)
    {
        if(pidArr[i].pid == rpid)
        {
            pidArr[i].bitmap = 0;
        }
    }
}
void * Multi_Thread(void * v)                          //function called by the created thread
{
    int rpid = allocate_pid();       //allocates a pid
	while (thread_number < size_of_thread)
    {
        pthread_mutex_lock(&mutex);     //mutex lock occurs
        if (thread_number >= size_of_thread)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        thread_number++;                    //threadVar increments at least once
        //sleep(100);
        printf("\n\t###############################################");
        printf("\n\t##   Thread %d    ##   Succesfully Created   ##",thread_number);
        pthread_mutex_unlock(&mutex);      //mutex now unlocked
    }
    //sleep(5);
    release_pid(rpid);           //pid released
}

int main()
{
    int i =0;
    printf("\n\tEnter Number of Threads You Want : ");
    scanf("%d",&size_of_thread);
    pthread_t thread[size_of_thread];
	printf("\n\t###############################################");
	printf("\n\t##                                           ##");
	printf("\n\t##          Creating %d Threads              ##",size_of_thread);
	printf("\n\t##                                           ##");
	printf("\n\t###############################################");
	
//   sleep(1000);        //delay only so that the above can be read in output screen before execution of the rest of the code
	
    for(i = 0; i < size_of_thread; i++)
    {
        pthread_mutex_init(&mutex, NULL);
        pthread_create(&thread[i], NULL, Multi_Thread, NULL);
        Multi_Thread(NULL);
    }
    for(i = 0; i < size_of_thread; i++)
    {
        pthread_join(thread[i], NULL);
        pthread_mutex_destroy(&mutex);
    }
	printf("\n\t###############################################");
    return 0;
}
