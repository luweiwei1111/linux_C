INCS = -I/usr/local/include
CFLAGS = $(INCS) -L/usr/local/lib -lzlog -lpthread

poll_server:poll_server.o zlog.o
	gcc -g -Wall -o poll_server -g poll_server.o zlog.o $(CFLAGS)

poll_server.o:poll_server.c
	gcc $(INCS) -c -g -o $@ $<

poll_client:poll_client.o zlog.o
	gcc -g -Wall -o poll_client -g poll_client.o zlog.o $(CFLAGS)

poll_client.o:poll_client.c
	gcc $(INCS) -c -g -o $@ $<

zlog.o:zlog.c
	gcc $(INCS) -c -g -o $@ $<

clean:
	rm zlog.o poll_client.o poll_client  poll_server poll_server.o
