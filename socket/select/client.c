#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include "zlog_api.h"
#define maxn 1100
#define IP  "127.0.0.1"
#define PORT 45178
#define MAXLINE 1024
#define LISTENQ 5
#define SIZE 10
#define BACKLOG 2

void handler_INT()
{
    LOG_ERROR("process exit, log fini!\n");
    log_fini();
    exit(1);
}

int main()
{
    if(log_init() != 0)
    {
        printf("log init error\n");
        return -1;
    }

    signal(SIGINT, handler_INT);
    
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        LOG_ERROR("socket error!\n");
        log_fini();
        exit(1);
    }
    struct sockaddr_in server;
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET,IP,&server.sin_addr);
    char buf[maxn];
    int connfd;
    connfd = connect(sockfd,(struct sockaddr*)&server,sizeof(server));
    if(connfd < 0)
    {
        LOG_ERROR("connect failure!\n");
        log_fini();
        return -1;
    }
    LOG_DEBUG("client send to server\n");
    LOG_DEBUG("please input something\n");
    scanf("%s",buf);
    write(sockfd,buf,maxn);
    char recvbuf[maxn];
    char sendbuf[maxn];
    fd_set readfd;
    int maxnum = 0;
    struct timeval T_time;
    int n;
    int sel_fd;
    while(1)
    {
        FD_ZERO(&readfd);
        FD_SET(sockfd,&readfd);
        maxnum = sockfd;
        T_time.tv_sec = 2;
        T_time.tv_usec = 0;
        sel_fd = select(maxnum + 1,&readfd,NULL,NULL,&T_time);
        if(sel_fd < 0)
        {
            continue;
        }
        else if(sel_fd == -1)
        {
            LOG_ERROR("select error!\n");
            log_fini();
            return -1;
        }
        if(FD_ISSET(sockfd,&readfd))
        {
            n = read(sockfd,recvbuf,maxn);
            if(n <= 0)
            {
                LOG_ERROR("server is closed!\n");
                close(sockfd);
                FD_CLR(sockfd,&readfd);
                log_fini();
                return -1;
            }
            LOG_DEBUG("recv message is %s\n", recvbuf);
            sleep(5);
            write(sockfd,buf,strlen(buf) + 1);
        }
    }

    close(sockfd);
    log_fini();
    return 0;
}