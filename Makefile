CC=gcc
CFLAGS= -c -Wall

all: minidata

minidata: main.o file.o db.o command.o task.o string.o data.o commit.o
	$(CC) -Wall main.o file.o db.o command.o task.o string.o data.o commit.o -o minidata

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

string.o: src/string.c
	$(CC) $(CFLAGS) src/string.c

data.o: src/data.c
	$(CC) $(CFLAGS) src/data.c

commit.o: src/commit.c
	$(CC) $(CFLAGS) src/commit.c

test: clean all
	minidata test/db_test.mncd

clean:
	rm -f *.o minidata