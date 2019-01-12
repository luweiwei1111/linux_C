INCS = -I/usr/local/include
CFLAGS = $(INCS) -L/usr/local/lib -lzlog -lpthread

epoll_server:epoll_server.o zlog.o
	gcc -g -Wall -o epoll_server -g epoll_server.o zlog.o $(CFLAGS)

epoll_server.o:epoll_server.c
	gcc $(INCS) -c -g -o $@ $<

epoll_client:epoll_client.o zlog.o
	gcc -g -Wall -o epoll_client -g epoll_client.o zlog.o $(CFLAGS)

epoll_client.o:epoll_client.c
	gcc $(INCS) -c -g -o $@ $<

zlog.o:zlog.c
	gcc $(INCS) -c -g -o $@ $<

clean:
	rm zlog.o epoll_client.o epoll_client  epoll_server epoll_server.o
