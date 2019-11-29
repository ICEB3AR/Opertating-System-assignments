#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define max  0x10
int counter = 0;
int in = 0;
int out = 0;

int buffer[max] = {0,};
int loop = 500000;
void * producer(void *data){
    for(int i=0;i<loop;i++){
    while(counter == 0x10)
        ;
    buffer[in]++;

    printf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12],buffer[13],buffer[14],buffer[15]);
    printf("++++++ [%02d] : %d\n\n",in,buffer[in]);    
    in = (in+1) %max;
    counter++;

    }
}
void * consumer(void *data){
    for(int i=0;i<loop;i++){
    while(counter == 0)
        ;
    int consumed = buffer[out];
    sleep(0.5);
    buffer[out]--;
    printf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12],buffer[13],buffer[14],buffer[15]);
    printf("------ [%02d] : %d\n\n",out,consumed);
    if(consumed != 1 ){
        printf("^ Race Condition\n\n\n");
    }
    out = (out + 1) %max;
    counter--;


    }
}

int main(){
    pthread_t p_thread[3];
    char p1[] = "producer";
    char p2[] = "consumer";
    char p3[] = "checker";
    int status;
    pthread_create(&p_thread[0],NULL,producer,(void*) p1);
    pthread_create(&p_thread[1],NULL,consumer,(void*) p2);
    //pthread_create(&p_thread[2],NULL,t_checker,(void*) p3);
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);
    //pthread_join(p_thread[2], (void **)&status);


}