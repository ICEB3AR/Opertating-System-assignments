#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#define REQ_FIFO_FILE_1   "/tmp/requestfifo1"
#define REQ_FIFO_FILE_2   "/tmp/requestfifo2"
#define RES_FIFO_FILE "/tmp/responsefifo"
#define  BUF_SIZE   2048
#define READMODE 1
#define WRITEMODE 0


char  buf[BUF_SIZE];
char filename[BUF_SIZE] = {0,};
char lengthString[128] = {0,};
int length=0;
char content[BUF_SIZE] = {0,};
int   requestfifoFd1;
int   requestfifoFd2;
int   responsefifoFd;
FILE* responseFile=NULL;
char *responseBuffer;
pid_t pid;
int status;

int openPipes(){
    //make pipe
    if(mkfifo(REQ_FIFO_FILE_1, 0666) == -1){
        perror("1 :mkfifo() error\n");
        unlink(REQ_FIFO_FILE_1);
        return -1;
    }
    //open file
    if((requestfifoFd1 = open(REQ_FIFO_FILE_1,O_RDWR))==-1){
        perror("1 :open() error\n");
        return -1;
    }
        //make pipe
    if(mkfifo(REQ_FIFO_FILE_2, 0666) == -1){
        perror("2 :mkfifo() error\n");
        unlink(REQ_FIFO_FILE_2);
        return -1;
    }
    //open file
    if((requestfifoFd2 = open(REQ_FIFO_FILE_2,O_RDWR))==-1){
        perror("2 :open() error\n");
        return -1;
    }
            //make pipe
    if(mkfifo(RES_FIFO_FILE, 0666) == -1){
        perror("3 :mkfifo() error\n");
        unlink(RES_FIFO_FILE);
        return -1;
    }
    //open file
    if((responsefifoFd = open(RES_FIFO_FILE,O_RDWR))==-1){
        perror("3 :open() error\n");
        return -1;
    }

}
int openResPipe(){
    if(-1 == (responsefifoFd=open(RES_FIFO_FILE,O_WRONLY))){
        perror("3 :open() error\n");
        return -1;
    }
}
int getRequest(){

    memset( buf, 0, BUF_SIZE);
    read( requestfifoFd2, buf, 4);
    if(!strncmp(buf,"REQR",4)) return READMODE;
    if(!strncmp(buf,"REQW",4)) return WRITEMODE;

    return -1;
}
void getFilename(){

    memset(filename, 0, BUF_SIZE); 
    read(requestfifoFd1,filename,BUF_SIZE);
    printf("file :%s\n",filename);
    return;
}
void getLength(){
    memset(lengthString, 0, 128); 
    read(requestfifoFd2,lengthString,128);
    length = atoi(lengthString);
}
void getContent(){
    memset( content, 0, BUF_SIZE);
    read( requestfifoFd2, content, BUF_SIZE);
    return;
}
void sendResponse(char *buffer,int size){
    //openResPipe();

        write(responsefifoFd,buffer,size);
}    
void readFileByte(FILE* fp,int length){
    responseBuffer = malloc(1024);
    char chr[2];
    for(int i=0;i<length;i++){
        if(!feof(fp)){
            chr[0] = fgetc(fp);
            strcat(responseBuffer,chr);
            //printf("%c\n",chr[0]);
        }
        else {
            responseBuffer[i-1]='\x00';
        }
    }
    return;
}
int readMode(){
    pid = fork();
	if(pid == -1) { 
		printf("fork() error\n");
		exit(0);
	}    
    //child
    if(pid == 0){
        printf("FORK!\n");
        getFilename();
        getLength();
        printf("READ %s %d bytes\n",filename,length);
        responseFile = fopen(filename,"r");
        if(!responseFile){
            printf("nofile");
            exit(1);
        }
        
        readFileByte(responseFile,length);
        fclose(responseFile);
        sendResponse(responseBuffer,length);
        free(responseBuffer);
        return READMODE;
    }
    //parent
    else{
        wait(&status);
        return 1;
    }

}
void writeFile(FILE* fd){
    int leng = strlen(content);
    responseBuffer = malloc(1024);
    fwrite(content,1,leng,fd);
    sprintf(responseBuffer,"%d",leng);
    fclose(fd);
}
int writeMode(){
    pid = fork();
	if(pid == -1) { 
		printf("fork() error\n");
		exit(0);
	}       
    //child
    if(pid == 0){
        printf("WRITEFORK!\n");
        getFilename();
        getContent();
        printf("WRITE %s to %s\n",content,filename);
        responseFile = fopen(filename,"w");
        if(!responseFile){
            printf("nofile");
            exit(1);
        }
        writeFile(responseFile);
        sendResponse(responseBuffer,strlen(responseBuffer));
        free(responseBuffer);
        return WRITEMODE;
    }
    //parent
    else{
        printf("PARENT");  
        wait(&status);    
        return 1;
    }

}
void unlinkPipes(){
        unlink(REQ_FIFO_FILE_1);
        unlink(REQ_FIFO_FILE_2);
        unlink(RES_FIFO_FILE);
        close(requestfifoFd1);
        close(requestfifoFd2);
        close(responsefifoFd);
}
int main(){

        int counter = 0;
        int mode=-1;

        char filename[1024];

        setvbuf(stdout, 0,1,0);
        setvbuf(stdin, 0,1,0);
        openPipes();
    while(1){


            mode = getRequest();

            if(mode == READMODE){
                readMode();
            }
            if(mode == WRITEMODE){
                writeMode();
            }


        }
        unlinkPipes();


}