#include "data.h"

static int  init()
{
    int sockfd;
    struct sockaddr_in client_in;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        fprintf(stderr,    "socket fail, errno %s\n",strerror(errno));
        return -1;
    }
    bzero(&client_in,sizeof(client_in));
    client_in.sin_family = AF_INET;
    client_in.sin_port = htons(PORT);
    inet_pton(AF_INET,IP,&client_in.sin_addr);
    connect(sockfd,(struct sockaddr*)&client_in,sizeof(client_in));
    return sockfd;
}
static void poll_conn(int sockfd)
{
    struct pollfd fdarray[2];
    char buf[maxn];
    int n;
    fdarray[0].fd = sockfd;
    fdarray[0].events = POLLIN;
    fdarray[1].fd = STDIN_FILENO;
    fdarray[1].events = POLLIN;
    for(;;)
    {
        printf("please input message:\n");
        poll(fdarray,2,-1);
        if(fdarray[0].revents & POLLIN)
        {
            n = read(sockfd,buf,maxn);
            if(n == 0)
            {
                fprintf(stderr,"server closed.\n");
                close(sockfd);
                return;
            }
            write(STDOUT_FILENO,buf,n);
            
        }
        if(fdarray[1].revents & POLLIN)
        {
            n = read(STDIN_FILENO,buf,maxn);
            if(n == 0)
            {
                 shutdown(sockfd,SHUT_WR);
                continue;
            }
            write(sockfd,buf,n);
        }
    }
}
int main()
{
    int sockfd = init();
    poll_conn(sockfd);
    close(sockfd);
    return 0;
}