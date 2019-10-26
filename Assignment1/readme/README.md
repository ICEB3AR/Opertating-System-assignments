# 운영체제 1차 과제물             

### 사이버 보안학과 201820665 하동현

## 구성

* server.c
* client.c
* 서버와 클라이언트를 컴파일하고 실행시키는 쉘스크립트
* server, client의 동작을 테스트하는 파이썬 스크립트


### 1. 클라이언트

클라이언트는 명령어 형식으로 입력을 받는다.

    USEAGE: [filename] -[w or r] [bytes if read, string if write]

파일이름을 입력하고 -옵션으로 write or read를 지정한다.

그 다음에는 read인경우 바이트 수, write인 경우에는 입력할 문자열을 적는다.


    helloworld.c -r 99
    flag -w FLAG{ASS1GNM3N7_F1N1SH!}

첫번째 명령은 ```helloworld.c```에서 ```99```바이트를 읽어라는 명령,
두번째는 ```flag```라는 파일에 ```FLAG{ASS1GNM3N7_F1N1SH!}```


클라이언트는 내부적으로 해당 명령어를 파싱해서 서버에게 pipe두개를 이용하여 Read,Write모드, 파일이름, 인자를 각각 전달한다.

### 2. 서버

클라이언트로부터 모드를 알리는 4바이트 문자열을 가장 처음 받는다.

서버는 해당 모드가 Read인지 Write인지 판별하고 각각 분기로 나뉘어 실행된다. 

이후 fork를 통해 자식프로세스를 생성하고 Read라면 file read를 수행, write라면 file write를 수행한다.

### 3. 실행방법

    gcc -w -o server server.c
    gcc -o client client.c

위와같이 컴파일 한뒤, server는 백그라운드로 동작시키고,
client를 실행시켜 이용한다.

    gcc -w -o server server.c
    gcc -o client client.c
    sudo rm -f /tmp/requestfifo1
    sudo rm -f /tmp/requestfifo2
    sudo rm -f /tmp/responsefifo
    ./server&
    sleep .3
    ./client

위의 내용의 run.sh 쉘스크립트를 실행하면, 간편하게 세팅을 할 수 있다.

### 4. 동작 확인

```simulation.py```는 pwntools를 이용해 간단하게 서버와 클라이언트를 검사하는 스크립트이다.

write와 read가 번갈아가면서 수행되며, 실제 write된 string과 read를 통해 읽어들인 string이 같은지 비교하여 성공률을 확인할 수 있는 스크립트이다.

    RESULT : total 500 trial 500 pass / 0 fail 100.0%

실제 read, write 각각 500번씩 총 1000번의 테스트를 해본 결과 pass는 500번, fail은 0번으로 매우 뛰어난 정확성을 보여주고 있다.