INCS = -I/usr/local/include
CFLAGS = $(INCS) -L/usr/local/lib -lzlog -lpthread

test_hello:test_hello.o
	gcc -o test_hello test_hello.o $(CFLAGS)
test_hello.o:test_hello.c
	gcc $(INCS) -c -o $@ $<

clean:
	rm test_hello.o test_hello 
