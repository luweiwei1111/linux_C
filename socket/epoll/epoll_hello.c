#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

#define _INT_BUF (255)

// epoll helloworld 练习
int main(void)
{
    int epfd, nfds, i, len;
    char buf[_INT_BUF];
    struct epoll_event ev;
    
    epfd = epoll_create(1); //监听一个描述符与. stdin
    ev.data.fd = STDIN_FILENO;
    ev.events = EPOLLIN; //使用默认的LT条件触发
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);
    
    // 10 表示等待 30s,过了直接退出
    for(;;){
        nfds = epoll_wait(epfd, &ev, 1, -1);
        for(i=0; i<nfds; ++i){
            if(ev.data.fd == STDIN_FILENO){
                len = read(STDIN_FILENO, buf, sizeof buf - 1);
                buf[len] = '\0';
                printf("%s" ,buf);
            }
        }

        //强加一个结束条件吧
        if(random() % 100 >= 90)
            break;
    }
    
    puts("Epoll Hello world is end!");
    // 只要是文件描述符都要释放
    close(epfd);
    return 0;
}