CC=gcc
CFLAGS= -c -Wall

all: minidata

minidata: main.o file.o db.o command.o task.o string.o data.o commit.o context.o lex.yy.o
	$(CC) -Wall main.o file.o db.o command.o task.o string.o data.o commit.o context.o lex.yy.o -o minidata

main.o: src/main.c
	$(CC) $(CFLAGS) src/main.c

file.o: src/util/file.c
	$(CC) $(CFLAGS) src/util/file.c

db.o: src/database/db.c
	$(CC) $(CFLAGS) src/database/db.c

command.o: src/command.c tool/lex.yy.c
	$(CC) $(CFLAGS) src/command.c
	$(CC) -c tool/lex.yy.c

task.o: src/task.c
	$(CC) $(CFLAGS) src/task.c

string.o: src/util/string.c
	$(CC) $(CFLAGS) src/util/string.c

data.o: src/util/data.c
	$(CC) $(CFLAGS) src/util/data.c

commit.o: src/database/commit.c
	$(CC) $(CFLAGS) src/database/commit.c

context.o: src/database/context.c
	$(CC) $(CFLAGS) src/database/context.c

tool/lex.yy.c: tool/miniscanner.l
	flex tool/miniscanner.l
	cp lex.yy.c tool

test: clean all
	minidata test/db_test.mncd
	minidata test/entity_test.mncd

clean:
	rm -f *.o minidata lex.yy.c