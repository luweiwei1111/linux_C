#include "data.h"
static void epoll_accept(int sockfd);
static void submit_to_check(int epollfd,struct epoll_event *events,int num,int sockfd,char *buf);
static void submit_to_read(int epollfd,int conn,char *buf);
static void submit_to_write(int epollfd,int conn,char *buf);
static void submit_to_accept(int epollfd,int conn);
static void change_event(int epollfd,int conn,int  ser_event);
static void change_delete(int epollfd,int conn,int ser_event);
static void add_event(int epollfd,int sockfd,int  ser_event);

static int init()
{
    int sockfd;
    struct sockaddr_in server_in;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        fprintf(stderr,"socket fail,error %s",strerror(errno));
        return -1;
    }
    bzero(&server_in,sizeof(server_in));
    server_in.sin_family = AF_INET;
    server_in.sin_port = htons(PORT);
    inet_pton(AF_INET,IP,&server_in.sin_addr);
    if(bind(sockfd,(struct sockaddr*)&server_in,sizeof(server_in)) == -1)
    {
        fprintf(stderr,"bind fail,error %s",strerror(errno));
        return -1;
    }
    listen(sockfd,BACKLOG);
    return sockfd;
}

static void epoll_accept(int sockfd)
{
    int epollfd;
    char buf[maxn];
    memset(buf,0,sizeof(buf));
    struct epoll_event events[_EVENTS];
    epollfd = epoll_create(EPOLLEVENTS);
    int ret;
    add_event(epollfd,sockfd,EPOLLIN);
    for(;;)
    {
        ret = epoll_wait(epollfd,events,_EVENTS,-1);
        submit_to_check(epollfd,events,ret,sockfd,buf);
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
        if((fd == sockfd) && (events[i].events&EPOLLIN))
        {
            submit_to_accept(epollfd,sockfd);
        }    
        else if(events[i].events & EPOLLIN)
                    {
                                submit_to_read(epollfd,fd,buf);
                    }
        else if(events[i].events & EPOLLOUT)
        {
            submit_to_write(epollfd,fd,buf);
        }
    }
}
static void submit_to_read(int epollfd,int conn,char *buf)
{
    int n;
    n = read(conn,buf,maxn);
    if(n == 0)
    {
        fprintf(stderr,"client close!\n");
        close(conn);
        change_delete(epollfd,conn,EPOLLIN);
    }
    else if(n == -1)
    {
        fprintf(stderr,"read error\n");
        close(conn);
        change_delete(epollfd,conn,EPOLLIN);
    }
    else
    {
        fprintf(stderr,"read message is %s\n",buf);
        change_event(epollfd,conn,EPOLLOUT);
    }
}
static void submit_to_write(int epollfd,int conn,char *buf)
{
    int n;
    n = write(conn,buf,strlen(buf));
    if(n == -1)
    {
        printf("server closed!\n");
        close(conn);
        change_delete(epollfd,conn,EPOLLOUT);
    }
    else
    {
        change_event(epollfd,conn,EPOLLIN);
    }
    memset(buf,'\0',sizeof(buf));
}
static void submit_to_accept(int epollfd,int conn)
{
    struct sockaddr_in server_in;
    bzero(&server_in,sizeof(server_in));
    socklen_t socklen;
    int afd;
    socklen = 1;
    if((afd = accept(conn,(struct sockaddr*)&server_in,&socklen)) == -1)
    {
        fprintf(stderr,"accept fail,error %s\n",strerror(errno));
        return;
    }
    else
    {
        printf("accept a new client:%s :%d\n",inet_ntoa(server_in.sin_addr),server_in.sin_port);
        add_event(epollfd,afd,EPOLLIN);
    }
}
static void change_event(int epollfd,int conn,int  ser_event)
{
    struct epoll_event eventfd;
    eventfd.data.fd = conn;
    eventfd.events = ser_event;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,conn,&eventfd);
}
static void change_delete(int epollfd,int conn,int ser_event)
{
    struct epoll_event eventfd;
    eventfd.data.fd = conn;
    eventfd.events = ser_event;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,conn,&eventfd);
}
static void add_event(int epollfd,int sockfd,int  ser_event)
{
    struct epoll_event eventfd;
    eventfd.data.fd = sockfd;
    eventfd.events = ser_event;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&eventfd);
}

int main()
{
    int sockfd;
    sockfd = init();
    epoll_accept(sockfd);
    close(sockfd);
    return 0;
}