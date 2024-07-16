ALL: main.o BlockQ.o easy_threadpool.o BlockQ.h easy_threadpool.h
	gcc main.o  BlockQ.o easy_threadpool.o  -o main -g -Wall -pthread

BlockQ.o: BlockQ.h BlockQ.c
	gcc -c BlockQ.c -Wall -g -pthread
testBlock.o: testBlock.c BlockQ.o BlockQ.h
	gcc -c testBlock.c -Wall -g -pthread
easy_threadpool.o: BlockQ.o BlockQ.h easy_threadpool.h easy_threadpool.c
	gcc -c easy_threadpool.c -Wall -g -pthread

main.o: BlockQ.o BlockQ.h easy_threadpool.o easy_threadpool.h
	gcc -c main.c -Wall -g -pthread

