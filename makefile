all: main.o write_file.o
	gcc -o control main.o
	gcc -o write write_file.o

main.o: main.c
	gcc -c main.c

write_file.o: write_file.c
	gcc -c write_file.c

clean:
	rm *.o
	rm *~
