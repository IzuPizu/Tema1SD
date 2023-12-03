CC=gcc
CFLAGS=-Wall


build: tema1.c 
	$(CC) $(CFLAGS) tema1.c -o tema1

run: build
	./tema1

clean:
	rm -f *.o exec

archive:
	zip -r tema1.zip Makefile README *.c *.h