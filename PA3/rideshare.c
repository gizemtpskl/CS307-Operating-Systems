#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>



int A_fans = 0; // A's count
int B_fans = 0; //B's count
int A_fan_count, B_fan_count;
int captain=0; 
pthread_mutex_t mutex_A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_B = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_AB = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_A, sem_B;




void * rideshare(void * argv);

int main( int argc, char* argv[])
{
    
    
    
    A_fan_count = atoi(argv[1]);
    B_fan_count = atoi(argv[2]);
    sem_init(&sem_A, 0, 0);
    sem_init(&sem_B, 0, 0);



    if((A_fan_count + B_fan_count)%4 == 0&&A_fan_count % 2==0 && B_fan_count % 2 == 0 )
    {
        char A = 'A';
        char B = 'B';
        
        pthread_t thread_A[A_fan_count]; 
        pthread_t thread_B[B_fan_count];
        
        for(int i=0;i<A_fan_count;i++)
        {
            //creating threads for every A fan
            pthread_create(&thread_A[i],NULL,rideshare, &A);
        }

        for(int i=0; i<B_fan_count; i++)
        {
            //creating threads for every B fan
            pthread_create(&thread_B[i],NULL,rideshare, &B);
        }
        for(int i=0; i<A_fan_count; i++)
        {
            pthread_join(thread_A[i],NULL);
        }
        for(int i=0; i< B_fan_count; i++)
        {
            pthread_join(thread_B[i],NULL);
        }
    }
    
    sem_destroy(&sem_A);
    sem_destroy(&sem_B);
    printf("The main terminates\n");

    return 0;
}


void * rideshare(void * argv)
{
   
    char teamname = * (char*)argv;
    
    if(teamname == 'A')
    {
        pthread_mutex_lock(&mutex_A);  //locking for the fan who is looking for a car
        printf("Thread ID: %ld, Team: %c, I am currently looking for a car\n", pthread_self(), teamname);
        
        A_fans++;

        if( B_fans >= 2 &&A_fans == 2 )
        {
            //if team A has 2 fans team B has more
            sem_post(&sem_A);
            sem_post(&sem_B);
            sem_post(&sem_A);
            sem_post(&sem_B);
            A_fans -= 2;
            B_fans-= 2;
        }
        
        else if(B_fans == 4)
        {
            //put all the team B fans into the same car
            sem_post(&sem_B);
            sem_post(&sem_B);
            sem_post(&sem_B);
            sem_post(&sem_B);
            B_fans -= 4;
        }

        else if(A_fans == 4)
        {
            //put all the team A fans into the same car
            sem_post(&sem_A);
            sem_post(&sem_A);
            sem_post(&sem_A);
            sem_post(&sem_A);
            A_fans -= 4;
        }
        pthread_mutex_unlock(&mutex_A);
        sem_wait(&sem_A);
    }
    else if(teamname == 'B')
    {
        pthread_mutex_lock(&mutex_B);
        printf("Thread ID: %ld, Team: %c, I am currently looking for a car\n", pthread_self(), teamname);
        B_fans++;
        
        if(B_fans == 2&& A_fans >= 2  )
        {
            sem_post(&sem_B);
            sem_post(&sem_A);
            sem_post(&sem_B);
            sem_post(&sem_A);
            A_fans -= 2;
            B_fans -= 2;
        }
        else if(B_fans == 4)
        {
            sem_post(&sem_B);
            sem_post(&sem_B);
            sem_post(&sem_B);
            sem_post(&sem_B);
            B_fans -= 4;
        }
        else if(A_fans == 4)
        {
            sem_post(&sem_A);
            sem_post(&sem_A);
            sem_post(&sem_A);
            sem_post(&sem_A);
            A_fans -= 4;
        }
        pthread_mutex_unlock(&mutex_B);
        sem_wait(&sem_B);
    }
    
    
    if(teamname == 'A' || teamname == 'B')
    {

        pthread_mutex_lock(&mutex_AB); //locking when fans start to find a car
        printf("Thread ID: %ld, Team: %c, I have found a spot on the car\n", pthread_self(), teamname);

        captain++; //counting fans to find the captain
        
       
        if(captain % 4 == 0 && captain != 0) //every time it is divisible to 4 it means it is the last one in car so it is the captain
        {
            printf("Thread ID: %ld, Team: %c, I am the captain and driving the car\n", pthread_self(), teamname);
        }

        
        pthread_mutex_unlock(&mutex_AB); //unlocking when the car is full
    }  
}