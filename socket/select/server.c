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
#include <unistd.h>
#include "zlog_api.h"
#define maxn 1100
#define PORT 45178
#define IP "127.0.0.1"
#define MAXLINE 1024
#define LISTENQ 5
#define SIZE 10
#define BACKLOG 2

typedef struct server_context_st
{
    int cli_num;        /*客户端个数*/
    int cli_fds[SIZE];  /*客户端的个数*/
    fd_set allfds;      /*句柄集合*/
    int maxfd;          /*句柄最大值*/
} server_context_st;

static int  init();
static void deal_maxfd(int sockfd);
static int accept_client(int sockfd);
static void recv_client_msg(fd_set *readfd);
static void submit_client_msg(int temp,char buf[]);
static server_context_st *server_client = NULL;

int Max(int a,int b);

int Max(int a,int b)
{
    return (a)>(b)?(a):(b);
}

static int server_init()
{
    server_client = (server_context_st *)malloc(sizeof(server_context_st));
    if(server_client == NULL)
    {
        LOG_ERROR("malloc failed");
        return -1;
    }
    memset(server_client, 0, sizeof(server_context_st));
    int i=0;
    for(;i<SIZE;i++)
    {
        server_client->cli_fds[i] = -1;
    }
    return 0;
}

/* socket、bind、listen */
static int init()
{
    /* 1. socket(int family,int type,int protocol);
    family ->协议类型
    AF_INET   ----> IPV4
    AF_INET6  ----> IPV6
    AF_ROUTE  ----> 路由套接口

    type->是指套接字类型
    SOCK_STREAM  --->  字节流套接字   TCP
    SOCK_DGRAM   --->  数据报套接字　　UDP
    SOCK_RAW     --->  原始套接字
    函数调用成功返回小的非负整数，文件描述符类型，否则返回-1表示调用失败 */
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        LOG_ERROR("socket error!\n");
        exit(1);
    }
    int listenfd;
    struct sockaddr_in server;
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET,IP,&server.sin_addr);

    /*bind(int sockfd,struct sockaddr *server,socklen_len addrlen);
    sockfd->socket的返回值
    server->表示指向于特定协议的地址结构的指针
    addrlen->表示该套接字的地址结构的长度*/
    if(bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr)) == -1)
    {
        LOG_ERROR("bind error:");
        return -1;
    }

    /*listen(int sockfd,int backlog)
    sockfd --->socket的返回值
    backlog --->规定了请求返回队列的最大连接数，它对队列中等待服务请求的数目进行限制，如果一个服务请求到来时，输入队列已满，该套接字将拒绝连接请求。
    listen(sockfd,5);
    对于一个监听套接字，内核要维护两个队列：未完成连接队列和已完成连接队列
    未完成连接队列：为每个请求建立连接的SYN分节设一个条目，服务器正等待完成三次握手，当前的套接字处在SYN_RECD状态
    已完成连接队列：为每个完成TCP三次握手的客户端开设一个条目，当前的套接字状态时ESTABLISHED。*/
    listen(sockfd,BACKLOG);
    return sockfd;
}

static void deal_maxfd(int sockfd)
{
    fd_set *readfd = &server_client->allfds;
    int sel_fd = 0;
    int clifd = -1;
    struct timeval T_time;
    while(1)
    {
        FD_ZERO(readfd);
        FD_SET(sockfd,readfd);
        server_client->maxfd = sockfd;
        T_time.tv_sec = 30;
        T_time.tv_usec = 0;
        
        int i;
        for(i=0;i<server_client->cli_num;i++)
        {
            clifd = server_client->cli_fds[i];
            FD_SET(clifd,readfd);
            server_client->maxfd =  Max(clifd,server_client->maxfd);
        }
        //retval = select(s_srv_ctx->maxfd + 1, readfds, NULL, NULL, &tv);
        sel_fd  = select(server_client->maxfd+1,readfd,NULL,NULL,&T_time);
        if(sel_fd == 0)
        {
            LOG_ERROR("time out!\n");
            continue;
        }
        else if(sel_fd == -1)
        {
            LOG_ERROR("something error!\n");
            return ;
        }
        if(FD_ISSET(sockfd,readfd))
        {
            /*监听客户端请求*/
            accept_client(sockfd);
        }
        else
        {
            /*接受处理客户端消息*/
            recv_client_msg(readfd);
        }
    }
}

/* accept */
static int accept_client(int sockfd)
{
    struct sockaddr_in server_c;
    socklen_t len;
    len = sizeof (server_c);
    int afd = -1;
    Loop:
    LOG_DEBUG("waiting ............................\n");
    /*accept(int sockfd,struct sockaddr *client,socklen_t *addrlen)
    sockfd 是socket()返回的套接字描述符,在调用listen的时候此套接字变成了监听套接字
    client 是返回对方的套接字和地址结构
    addrlen 是对应结构的长度
    accetp()函数返回，已连接套接字描述符。
　　注：一个服务器只能有一个监听套接字，而且会一直存在，直到服务器关闭，
而已连接套接字描述符是内核为每个被接受的客户都创建一个，当服务器完成与客户的数据传输时，要关闭连接套接字，
所以监听套接字接受客户的链接请求，已连接套接字描述符负责对应客户进行数据传送。*/
    afd = accept(sockfd,(struct sockaddr*)&server_c,&len);
    if(afd == -1)
    {
        if(errno == EINTR)
        {
            goto Loop;
        }
        else
        {
             fprintf(stderr, "accept fail,error:%s\n", strerror(errno));
            return -1;
        }
    }

    LOG_DEBUG("accept successful!\n");
    int i=0;
    for(i=0;i<SIZE;i++)
    {
        if(server_client->cli_fds[i] < 0)
        {
            server_client->cli_fds[i] = afd;
            server_client->cli_num ++;
            break;
        }
    }
    if(i == SIZE)
    {
        LOG_ERROR("too many client to accept!\n");
        return -1;
    }
}

/* read */
static void recv_client_msg(fd_set *readfd)
{
    int i=0;
    int temp;
    char buf[maxn];
    for(i=0;i<server_client->cli_num;i++)
    {
        temp = server_client->cli_fds[i];
        if(temp < 0)
        {
            continue;
        }
        if(FD_ISSET(temp,readfd))
        {
            int n = read(temp,buf,maxn);
            if(n <= 0)
            {
                FD_CLR(temp,&server_client->allfds);
                close(temp);
                server_client->cli_fds[i] = -1;
                continue;
            }
            submit_client_msg(temp,buf);
        }
    }
}

/* write */
static void submit_client_msg(int temp,char buf[])
{
    assert(buf);
    LOG_DEBUG("receive message is %s\n",buf);
    write(temp,buf,strlen(buf)+1);
    return ;
}

int main()
{
    if(log_init() != 0)
    {
        printf("log init error\n");
        return -1;
    }

    if(server_init() == -1)
    {
        LOG_ERROR("init error\n");
        return -1;
    }

    int sockfd;
    sockfd = init();
    deal_maxfd(sockfd);
    close(sockfd);

    log_fini();
    return 0;
}