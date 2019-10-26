#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define REQ_FIFO_FILE_1   "/tmp/requestfifo1"
#define REQ_FIFO_FILE_2   "/tmp/requestfifo2"
#define RES_FIFO_FILE "/tmp/responsefifo"
#define READMODE 1
#define WRITEMODE 0

char mode=-1;
char filename[1024];
int length=1024;
char lengthString[128];
char content[1024]={0,};
int requestPipe1;
int requestPipe2;
int responsePipe;
int readCommand(){
    puts("USEAGE: [filename] -[w or r] [bytes if read, string if write]");
    scanf("%s -%c",filename,&mode);
    getchar();
    if(mode == 'r'){
        scanf("%d",&length);
        sprintf(lengthString, "%d", length);
        return READMODE;
    }
    if(mode == 'w'){
        scanf("%[^\n]",content);
        return WRITEMODE;
    }
}
void sendRequest(int mode){
    switch(mode){
        case READMODE:
            write(requestPipe2,"REQR",4);
            write(requestPipe1,filename,strlen(filename));
            write(requestPipe2, lengthString,strlen(lengthString));
            break;

        case WRITEMODE:
            write(requestPipe2,"REQW",4);
            write(requestPipe1,filename,strlen(filename));
            write(requestPipe2, content,strlen(content));
            break;
        
    }
}
int openPipes(){
    if(-1 == (requestPipe1=open(REQ_FIFO_FILE_1,O_WRONLY))){
        perror("open() error\n");
        return -1;
    }
    if(-1 == (requestPipe2=open(REQ_FIFO_FILE_2,O_WRONLY))){
        perror("open() error\n");
        return -1;
    }

    //open file
    if((responsePipe = open(RES_FIFO_FILE,O_RDWR))==-1){
        perror("3 :open() error\n");
        return -1;
    }

}
void readFileByte(FILE* fp,int length){
    char *content1 = malloc(length+0x10);
    char chr[2];
    for(int i=0;i<length;i++){
        if(!feof(fp)){
            chr[0] = fgetc(fp);
            strcat(content1,chr);
            printf("%s\n",content1);
        }
        else return;
    }
}
void getResponse(){
    while(1){/*
        memset( content, 0, 1024);
        read( responsePipe, content, 1024);

            printf("%s\n",content);
            return;*/
        if(content!=NULL){
            memset( content, 0, 1024);    
            read( responsePipe, content, length);
            printf("response: %s\n",content);
            return;
            }
    }
}
int main(){
    while(1){
        char str[2048];
        int modeFlag;
        setvbuf(stdout, 0,1,0);
        setvbuf(stdin, 0,1,0);
        openPipes();

        modeFlag = readCommand();
        sendRequest(modeFlag);
        getResponse();

        close(requestPipe1);
        close(requestPipe2);
        close(responsePipe);
    }
}