#ifndef DATA_H
#define DATA_H
#include <string.h>
#include <stdio.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include "zlog_api.h"
#define maxn 1100
#define MAXLINE 10
#define LISTEN 10
#define IP "127.0.0.1"
#define PORT 4578
#define BACKLOG 5
#define INFTIM -1
#endif