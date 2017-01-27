gcc -g -c -std=c99 -lcunit ../src/ringbuf.c -o ringbuf.o
gcc -g -c -std=c99 -lcunit test.c -o test.o 
gcc -g -std=c99 -lcunit test.o ringbuf.o -o test
