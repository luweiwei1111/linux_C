INCS = -I/usr/local/include
CFLAGS = $(INCS) -L/usr/local/lib -lzlog -lpthread

server:server.o zlog.o
	gcc -g -Wall -o server -g server.o zlog.o $(CFLAGS)

server.o:server.c
	gcc $(INCS) -c -g -o $@ $<

client:client.o zlog.o
	gcc -g -Wall -o client -g client.o zlog.o $(CFLAGS)

client.o:client.c
	gcc $(INCS) -c -g -o $@ $<

zlog.o:zlog.c
	gcc $(INCS) -c -g -o $@ $<

clean:
	rm zlog.o server.o server client.o client
