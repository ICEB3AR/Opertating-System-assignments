#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define max  0x10


int buffer = 0;
int loop = 500000;
void * writer(){
    for(int i=0;i<loop;i++){
        buffer++;
        printf("write : %06d\n\n",buffer);    

    }
}
void * reader(){
    for(int i=0;i<loop;i++){
        printf("read  : %06d\n\n",buffer);    
    }
}

int main(){
    pthread_t p_thread[3];
    char p1[] = "writer";
    char p2[] = "reader1";
    char p3[] = "reader2";
    int status;
    pthread_create(&p_thread[0],NULL,writer,(void*) p1);
    pthread_create(&p_thread[1],NULL,reader,(void*) p2);
    pthread_create(&p_thread[2],NULL,reader,(void*) p3);
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);
    pthread_join(p_thread[2], (void **)&status);


}