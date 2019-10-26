gcc -w -o server server.c
gcc -o client client.c
sudo rm -f /tmp/requestfifo1
sudo rm -f /tmp/requestfifo2
sudo rm -f /tmp/responsefifo
./server&
sleep .3
./client
