epoll模型

int epoll_create(int maxevent) //创建一个epoll的句柄 然后maxevent表示监听的数目的大小
int epoll_ctl(int epollfd,int op,int fd,struct epoll_event *event) //epoll的事件注册函数
/*
epollfd 为epoll_create()的返回值
op 表示为相应的操作
fd 为套接字
*event 表示监听什么事件
*/
OP 有
EPOLL_CTL_ADD：注册新的fd到epfd中；
EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
EPOLL_CTL_DEL：从epfd中删除一个fd；
int epoll_wait(int epollfd,struct epoll_event *event,int maxevents,int timeout);
/*
epollfd	epoll_create的返回值
*event	从内核得到时间的集合	
maxevents	告诉内核event多大 不能大于epoll_create()中的maxevent
timeout	表示等待的时间-1表示阻塞 0表示立即返回
*/

epoll_event结构体
1 struct epoll_event {
2   __uint32_t events;  /* Epoll events */
3   epoll_data_t data;  /* User data variable */
4 };
typedef union epoll_data
{
   void *ptr;
   int fd;
   uint32_t u32;
   uint64_t u64;
} epoll_data_t;

其中epoll_event 结构体中的events参数的值是宏定义：
EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里