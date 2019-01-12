#include "data.h"
static void deal_message(struct pollfd *fd_array,int num);
static void poll_accept(int sockfd);

static int init()
{
    struct sockaddr_in server_in;
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        fprintf(stderr,"socket fail,error :%s\n",strerror(errno));
        return -1;
    }
    bzero(&server_in,sizeof(server_in));
    server_in.sin_family = AF_INET;
    server_in.sin_port = htons(PORT);
    inet_pton(AF_INET,IP,&server_in.sin_addr);
    if(bind(sockfd,(struct sockaddr*)&server_in,sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr,"bind fail,error:%s\n",strerror(errno));
        return -1;
    }
    listen(sockfd,BACKLOG);
    return sockfd;
}

static void poll_accept(int sockfd)
{
    struct pollfd fd_array[maxn];
    int afd;
    int pollfd;
    struct sockaddr_in client_in;
    bzero(&client_in,sizeof(client_in));
    int i = 0;
    fd_array[0].fd = sockfd;
    fd_array[0].events = POLLIN;
    for(i=1;i<maxn;i++)
    {
        fd_array[i].fd = -1;
    }
    int num = 0;
    int len = sizeof(client_in);
    for(;;)
    {
        pollfd = poll(fd_array,num+1,INFTIM); //无限等待
        if(pollfd == -1)
        {
            fprintf(stderr,"poll fail,error %s\n",strerror(errno));
            return;
        }
        if(pollfd == 0)
        {
            continue;
        }
        if(fd_array[0].revents & POLLIN) //判断实际发生的事件是否为普通或优先级带数据可读
        {
            if((afd = accept(sockfd,(struct sockaddr*)&client_in,&len)) == -1)
            {
                if(afd == EINTR)
                {
                    continue;
                }
                else 
                {
                    perror("accept error!");
                    return ;
                }
            }
            
            fprintf(stdout,"accept a new client: %s:%d\n", inet_ntoa(client_in.sin_addr),client_in.sin_port);
            for(i =1;i<maxn;i++)
            {
                if(fd_array[i].fd < 0)
                {
                    fd_array[i].fd = afd;
                    break;
                }
            }
            if(i == maxn)
            {
                printf("too many to server!\n");
                close(afd);
                return ;
            }
            fd_array[i].events = POLLIN;
            if(i > num ) 
                num = i;
            --pollfd;
        }
        deal_message(fd_array,num);
    }
}

static void deal_message(struct pollfd *fd_array,int num)
{
    int i,n;
    char buf[maxn+20];
    char bbuf[maxn];
    memset(buf,'\0',sizeof(buf));
    memset(bbuf,'\0',sizeof(bbuf));
    for(i=1;i<=num;i++) //轮询的方式
    {
        if(fd_array[i].fd < 0)
            continue;
        if(fd_array[i].revents&POLLIN)
        {
            n = read(fd_array[i].fd,bbuf,maxn);
            if(n == 0)
            {
                close(fd_array[i].fd);
                fd_array[i].fd = -1;
                continue;
            }
            sprintf(buf,"client %d say %s",i,bbuf);
            n += 15;
            write(STDOUT_FILENO,buf,n);
            write(fd_array[i].fd,buf,n);
        }
    }
}
int main()
{
    int sockfd = init();
    poll_accept(sockfd);
    close(sockfd);
    return 0;
}