CC=gcc
FLAGS=-g -Wall

main: main.o dictionary.o
	$(CC) $(FLAGS) -o main main.o dictionary.o
	make clean
main.o: main.c dictionary.h dictionary.c
	$(CC) $(FLAGS) -c main.c
dictionary.o: dictionary.h dictionary.c
	$(CC) $(FLAGS) -c dictionary.c
clean:
	del *.o