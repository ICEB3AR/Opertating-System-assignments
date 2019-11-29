# 운영체제 2차 과제물             

### 사이버 보안학과 201820665 하동현

## 구성

* producer_consumer_no_sync.c
* producer_consumer_sync.c
* reader_writer_no_sync.c
* reader_writer_sync.c

### 1. producer consumer

#### producer

```producer```는 0으로 초기화 된 0x10개의 버퍼를 한칸씩 돌면서, 1씩 증가시키는 역할을 한다.

counter가 버퍼 사이즈와 같아지면 ```consumer```를 기다린 뒤 다시 동작한다.

    printf("++++++ [%02d] : %d\n\n",in,buffer[in]);

구문을 통해 몇번째 버퍼에 어떤 값이 들어갔는지 확인 할 수 있다.


#### consumer

```consumer```는 0으로 초기화 된 0x10개의 버퍼를 한칸씩 돌면서, 1씩 감소시키는 역할을 한다.

counter가 0이 되면 ```producer```를 기다린 뒤 다시 동작한다.

    printf("------ [%02d] : %d\n\n",out,consumed);
    
구문을 통해 몇번째 버퍼에 어떤 값이 들어갔는지 확인 할 수 있다.


또한 이론상 항상 1을 받아와야 하기 때문에, 아래와 같은 구문으로 race condition을 확인할 수 있다.

    if(consumed != 1 ){
        printf("^ Race Condition\n\n\n");
    }

#### 동기화

```semaphore```를 이용하여 동기화를 구현하였다.

    //선언
    sem_t semaphore;

    //초기화
    sem_init(&semaphore,0,1);


    //함수의 시작
    sem_wait(&semaphore);

    //critical section

    sem_post(&semaphore);
    //함수의 끝

위와 같은 방식으로 구현하였다.

### 2. reader writer

#### writer


```writer```은 하나의 int variable을 1 증가시키는 역할을 한다.

    printf("write : %06d\n\n",buffer);  

구문을 통해 변수에 어떤 값이 들어갔는지 확인 할 수 있다.


#### reader


```reader```은 하나의 int variable을 읽어오는 역할을 한다.

    printf("read : %06d\n\n",buffer);  

구문을 통해 변수에서 읽어온 값을 확인 할 수 있다.

#### 동기화

```mutex lock```을 이용하여 동기화를 구현하였다.


    //선언
    pthread_mutex_t mutex_lock;

    //초기화
    pthread_mutex_init(&mutex_lock, NULL);

    //함수의 시작
    pthread_mutex_lock(&mutex_lock);

    //critical section

    pthread_mutex_unlock(&mutex_lock);
    //함수의 끝

위와 같은 방식으로 구현하였다.


### 3. 실행방법

    gcc -o producer_consumer_no_sync producer_consumer_no_sync.c -lpthread

    gcc -o producer_consumer_sync producer_consumer_sync.c -lpthread

    gcc -o reader_writer_no_sync reader_writer_no_sync.c -lpthread

    gcc -o reader_writer_sync reader_writer_sync.c -lpthread


```no_sync```가 붙은 바이너리는 동기화 기능이 없는 버전, ```sync```가 붙은 바이너리는 동기화 기능을 추가한 버전이며 별도의 조작없이 바이너리를 실행시키면 된다.

