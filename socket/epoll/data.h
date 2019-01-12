#ifndef DATA_H
#define DATA_H
#include <string.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "zlog_api.h"

#define IP "127.0.0.1"
#define PORT 4578
#define maxn 1100
#define MAXLINE 100
#define BACKLOG 5
#define EPOLLEVENTS 1120
#define _EVENTS 100
#endif