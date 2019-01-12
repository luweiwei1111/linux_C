#include "data.h"

static void connect_server(int sockfd);
static void submit_to_check(int epollfd,struct epoll_event *events,int num,int sockfd,char *buf);
static void submit_to_read(int epollfd,int fd,int sockfd,char *buf);
static void submit_to_write(int epollfd,int fd,int sockfd,char *buf);
static void change_event(int epollfd,int sockfd,int ser_event);
static void change_delete(int epollfd,int sockfd,int ser_event);
static void add_event(int epollfd,int sockfd,int ser_event);

static int init()
{
    struct sockaddr_in client_in;
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        fprintf(stderr,"socket fail,error %s\n",strerror(errno));
        return -1;
    }
    bzero(&client_in,sizeof(client_in));
    client_in.sin_family = AF_INET;
    client_in.sin_port = htons(PORT);
    inet_pton(AF_INET,IP,&client_in.sin_addr);
    connect(sockfd,(struct sockaddr*)&client_in,sizeof(client_in)) ;
    return sockfd;
}
static void connect_server(int sockfd)
{
    int epollfd;
    struct epoll_event events[_EVENTS];
    epollfd = epoll_create(EPOLLEVENTS);
    char buf[maxn];
    int conn;
    memset(buf,'\0',sizeof(buf));
    add_event(epollfd,STDIN_FILENO,EPOLLIN);
    for(;;)
    {
        fprintf(stderr,"Please input message!\n");
        conn = epoll_wait(epollfd,events,_EVENTS,-1);
        submit_to_check(epollfd,events,conn,sockfd,buf);
    }
    close(epollfd);
}
static void submit_to_check(int epollfd,struct epoll_event *events,int num,int sockfd,char *buf)
{
    int i;
    int fd;
    for(i=0;i<num;i++)
    {
        fd = events[i].data.fd;
        if(events[i].events & EPOLLIN)
        {
            submit_to_read(epollfd,fd,sockfd,buf);
        }
        else if(events[i].events & EPOLLOUT)
        {
            submit_to_write(epollfd,fd,sockfd,buf);
        }
    }
}
static void submit_to_read(int epollfd,int fd,int sockfd,char *buf)
{
    memset(buf,'\0',sizeof(buf));
    int n;
    n = read(fd,buf,maxn);
    if(n == 0)
    {
        fprintf(stderr,"server close.\n");
        close(fd);
    }
    else if(n == -1)
    {
        perror("read error:");
        close(fd);
    }
    else 
    {
        if(fd == STDIN_FILENO)
            add_event(epollfd,sockfd,EPOLLOUT);
        else 
        {
            change_delete(epollfd,sockfd,EPOLLIN);
            add_event(epollfd,STDOUT_FILENO,EPOLLOUT);
        }
    }
}
static void submit_to_write(int epollfd,int fd,int sockfd,char *buf)
{
    int n;
    n = write(fd,buf,strlen(buf));
    if(n == -1)
    {
        perror("write error:");
        close(fd);
    }
    else 
    {
        if(fd == STDOUT_FILENO)
        {
            change_delete(epollfd,fd,EPOLLOUT);
        }
        else 
        {
            change_event(epollfd,fd,EPOLLIN);
        }
    }
    memset(buf,0,sizeof(buf));
}
static void change_event(int epollfd,int sockfd,int ser_event)
{
    struct epoll_event eventfd;
    eventfd.data.fd = sockfd;
    eventfd.events = ser_event;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,sockfd,&eventfd);
}
static void change_delete(int epollfd,int sockfd,int ser_event)
{
    struct epoll_event eventfd;
    eventfd.data.fd = sockfd;
    eventfd.events = ser_event;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,sockfd,&eventfd);
}

static void add_event(int epollfd,int sockfd,int ser_event)
{
    struct epoll_event eventfd;
    eventfd.data.fd = sockfd;
    eventfd.events = ser_event;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&eventfd);
}
int main()
{
    int sockfd = init();
    connect_server(sockfd);
    close(sockfd);
    return 0;
}