poll模型

poll()机制和select()机制是相似的，都是对多个描述符进行轮询的方式。
不同的是poll()没有描述符数目的限制。
是通过struct pollfd结构体，对每个描述符进行轮询的

struct pollfd fdarray
{
　　int fd;    /*文件描述符*/
　　short events; /*表示等待的事件*/
　　short revents;/*表示返回事件即实际发生的事件*/
};

 每一个pollfd结构体指定了一个被监视的文件描述符，可以传递多个结构体，指示poll()监视多个文件描述符。每个结构体的events域是监视该文件描述符的事件掩码，由用户来设置这个域。revents域是文件描述符的操作结果事件掩码，内核在调用返回时设置这个域。events域中请求的任何事件都可能在revents域中返回。合法的事件如下：

　　POLLIN 　　　　　　　有数据可读。
　　POLLRDNORM 　　　　  有普通数据可读。
　　POLLRDBAND　　　　　 有优先数据可读。
　　POLLPRI　　　　　　　 有紧迫数据可读。
　　POLLOUT　　　　　　   写数据不会导致阻塞。
　　POLLWRNORM　　　　　  写普通数据不会导致阻塞。
　　POLLWRBAND　　　　　  写优先数据不会导致阻塞。
　　POLLMSGSIGPOLL 　　　 消息可用。

　　此外，revents域中还可能返回下列事件：
　　POLLER　　   指定的文件描述符发生错误。
　　POLLHUP　　 指定的文件描述符挂起事件。
　　POLLNVAL　　指定的文件描述符非法。

这些事件在events域中无意义，因为它们在合适的时候总是会从revents中返回。
　　使用poll()和select()不一样，你不需要显式地请求异常情况报告。
　　POLLIN | POLLPRI等价于select()的读事件，POLLOUT |POLLWRBAND等价于select()的写事件。POLLIN等价于POLLRDNORM |POLLRDBAND，而POLLOUT则等价于POLLWRNORM。例如，要同时监视一个文件描述符是否可读和可写，我们可以设置 events为POLLIN |POLLOUT。在poll返回时，我们可以检查revents中的标志，对应于文件描述符请求的events结构体。如果POLLIN事件被设置，则文件描述符可以被读取而不阻塞。如果POLLOUT被设置，则文件描述符可以写入而不导致阻塞。这些标志并不是互斥的：它们可能被同时设置，表示这个文件描述符的读取和写入操作都会正常返回而不阻塞。

　　timeout参数指定等待的毫秒数，无论I/O是否准备好，poll都会返回。timeout指定为负数值表示无限超时，使poll()一直挂起直到一个指定事件发生；timeout为0指示poll调用立即返回并列出准备好I/O的文件描述符，但并不等待其它的事件。这种情况下，poll()就像它的名字那样，一旦选举出来，立即返回。


　　返回值和错误代码
　　成功时，poll()返回结构体中revents域不为0的文件描述符个数；如果在超时前没有任何事件发生，poll()返回0；失败时，poll()返回-1，并设置errno为下列值之一：
　　EBADF　　       一个或多个结构体中指定的文件描述符无效。
　　EFAULTfds　　 指针指向的地址超出进程的地址空间。
　　EINTR　　　　  请求的事件之前产生一个信号，调用可以重新发起。
　　EINVALnfds　　参数超出PLIMIT_NOFILE值。
　　ENOMEM　　     可用内存不足，无法完成请求。

例子是从客户端发送信息，在服务器端显示并回射