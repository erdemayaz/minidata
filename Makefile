CC=gcc
CFLAGS= -c -Wall

all: minidata

minidata: main.o file.o db.o command.o task.o
	$(CC) main.o file.o db.o command.o task.o -o minidata

main.o: src/main.c
	$(CC) $(CFLAGS) src/main.c

file.o: src/file.c
	$(CC) $(CFLAGS) src/file.c

db.o: src/db.c
	$(CC) $(CFLAGS) src/db.c

command.o: src/command.c
	$(CC) $(CFLAGS) src/command.c

task.o: src/task.c
	$(CC) $(CFLAGS) src/task.c

clean:
	rm -f *.o minidata