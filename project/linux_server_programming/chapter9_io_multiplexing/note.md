# 第九章 I/O复用

## 9.1 select系统调用

### 1、函数原型

```C
#include＜sys/select.h＞
int select(int nfds,fd_set* readfds,fd_set* writefds,fd_set* exceptfds, struct timeval*timeout);
```

### 2、结构体 fd_set 定义

这即是位图的实现，fds_bits共16个元素，每一个元素为64bits的long int类型的数，该元素中的每一位对应一个文件描述符。文件描述符的范围是[0, 1024)，每一个文件描述符的取值为0或者1，即表示该文件描述符是否在位图集合中。fd_mask不一定是long int，也可以是int，这样的话NFDBITS就跟着变，使用long int可能是考虑到效率。

```c
/* Number of descriptors that can fit in an 'fd_set'.  */
#define FD_SETSIZE 1024
/* The fd_set member is required to be an array of longs.  */
typedef long int fd_mask;  // typedef的优势显露出来，这里改变，剩下的代码不用变
/* It's easier to assume 8-bit bytes than to get CHAR_BIT.  
*  一个字节是8位，这是死的，因此这里可以直接写8
*/
#define NFDBITS	(8 * (int)sizeof(fd_mask))  // 8*8==64，每一个篮子大小
#define	FD_ELT(d)	((d) / NFDBITS)  // 确定在哪一个篮子中
#define	FD_MASK(d)	((fd_mask) (1UL << ((d) % NFDBITS)))  // 在指定篮子中的偏移

/* fd_set for select and pselect.  */
typedef struct {
    fd_mask fds_bits[FD_SETSIZE / NFDBITS];  // 1024/64==16, sizeof(fd_set)==16*8
#define FDS_BITS(set) ((set)->fds_bits)
 } fd_set;

/* Access macros for 'fd_set'.  */
#define FD_ZERO(s)
  do {									      
    unsigned int i;							      
    fd_set *arr = (s);						      
    for (i = 0; i < sizeof (fd_set) / sizeof (fd_mask); ++i)  // 注意这里不能是sizeof(arr)因为会返回4/8byte     
      FDS_BITS(arr)[i] = 0;					      
  } while (0)
#define FD_SET(d, s) 
  ((void) (FDS_BITS(s)[FD_ELT(d)] |= FD_MASK(d)))
#define FD_CLR(d, s) 
  ((void) (FDS_BITS(s)[FD_ELT(d)] &= ~FD_MASK(d)))
#define FD_ISSET(d, s) 
  ((FDS_BITS(s)[FD_ELT(d)] & FD_MASK(d)) != 0)
```

## 9.3 epoll系列系统调用

### 1、函数原型

```c
int epoll_create(int size)  // 创建epollfd
/* 对epfd指定内核事件表进行op操作，操作对象是fd上的事件events */
int epoll_ctl(int epfd, int op, int fd, struct epoll_event* events) 
/* 返回epfd指定内核事件表上发生的事件数量，将发生的事件存储于epoll_event结构体数组变量events中 */
int epoll_wait(int epfd, epoll_event *events, int maxevents, int timeout)
```

epoll_event结构体的定义：

```c
typedef union epoll_data
{
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event
{
  uint32_t events;	/* Epoll events */
  epoll_data_t data;	/* User data variable */
} EPOLL_PACKED;
```

EPOLL_EVENTS如下所示:

```c
enum EPOLL_EVENTS {
    EPOLLIN = 0x001,  // 二进制, 0...0, 0...0, 0...0, 00000001; 十进制, 1
#define EPOLLIN EPOLLIN
    EPOLLPRI = 0x002,  // 0...0, 0...0, 0...0, 000000010; 2
#define EPOLLPRI EPOLLPRI
    EPOLLOUT = 0x004,  // 0...0, 0...0, 0...0, 00000100; 4
#define EPOLLOUT EPOLLOUT
    EPOLLRDNORM = 0x040, // 0...0, 0...0, 0...0, 01000000; 64
#define EPOLLRDNORM EPOLLRDNORM
    EPOLLRDBAND = 0x080,  // 0...0, 0...0, 0...0, 10000000; 128
#define EPOLLRDBAND EPOLLRDBAND
    EPOLLWRNORM = 0x100,  // 0...0, 0...0, 00000001, 0...0; 256
#define EPOLLWRNORM EPOLLWRNORM
    EPOLLWRBAND = 0x200,  // 0...0, 0...0, 00000010, 0...0; 512
#define EPOLLWRBAND EPOLLWRBAND
    EPOLLMSG = 0x400,  // 0...0, 0...0, 00000100, 0...0; 1024
#define EPOLLMSG EPOLLMSG
    EPOLLERR = 0x008,  // 0...0, 0...0, 0...0, 00001000; 8
#define EPOLLERR EPOLLERR
    EPOLLHUP = 0x010,  // 0...0, 0...0, 0...0, 00010000; 16
#define EPOLLHUP EPOLLHUP
    EPOLLRDHUP = 0x2000,  // 0...0, 0...0, 00100000, 0...0; 8192
#define EPOLLRDHUP EPOLLRDHUP
    EPOLLEXCLUSIVE = 1u << 28,  // 00010000, 0...0, 0...0, 0...0; 2^28
#define EPOLLEXCLUSIVE EPOLLEXCLUSIVE
    EPOLLWAKEUP = 1u << 29,  // 00100000, 0...0, 0...0, 0...0; 2^29
#define EPOLLWAKEUP EPOLLWAKEUP 
    EPOLLONESHOT = 1u << 30,  // 01000000, 0...0, 0...0, 0...0; 2^30
#define EPOLLONESHOT EPOLLONESHOT
    EPOLLET = 1u << 31   // 10000000, 0...0, 0...0, 0...0; 2^31
#define EPOLLET EPOLLET
 };
```

### 2、telnet server

#### 1) LT mode

client input:

```shell
xs@xslab:~$ telnet 127.0.0.1 50000
Trying 127.0.0.1...
Connected to 127.0.0.1.
Escape character is '^]'.
hello lt and et learn
```

server output:

```shell
./LT_ET_test 127.0.0.1 50000
in main(), listenfd is 3
in lt(), number of events is 1  # client telnet trigger, listenfd上的事件
in lt(), connfd is 5
in lt(), number of events is 1  # 以下这四行是connfd上的事件
in lt(), events[i].data.fd is 5
in lt(), event trigger once
in lt(), get 9 bytes of content:_hello et _
in lt(), number of events is 1  # connfd上的事件，因为没有读取完connfd上的数据
in lt(), events[i].data.fd is 5
in lt(), event trigger once
in lt(), get 9 bytes of content:_and lt le_
in lt(), number of events is 1  # connfd上的事件，继续读取
in lt(), events[i].data.fd is 5
in lt(), event trigger once
in lt(), get 5 bytes of content:_arn
_ #\r\n, 所以换行之后才显示_
# printf中的\n
```

#### 2) ET mode

server output:

```shell
./LT_ET_test 127.0.0.1 50000
in main(), listenfd is 3
event trigger once.
get 9 bytes of content: _hello et _
get 9 bytes of content: _and lt le_
get 5 bytes of content: _arn
_
read later.
# printf \n
```

### 3、EPOLLONESHOT事件

client input:

```shell
xs@xslab:~/clash$ telnet 127.0.0.1 50000
Trying 127.0.0.1...
Connected to 127.0.0.1.
Escape character is '^]'.
hello
jjj  # 输入 'hello' 后5秒内输入 'jjj' 
new data
hahaah
Connection closed by foreign host.  # server closed connection
```

server output:

```shell
./epolloneshot_test 127.0.0.1 50000
start new thread 126484182206016 to receive data on fd: 5
get content: hello

get content: jjj  # 在5秒接收到新数据到来，还是当前的线程负责处理


read later.
end thread receiving data on fd: 5  # 继上次接收到数据后5秒内没有新数据的到来，结束该线程
start new thread 126484100417088 to receive data on fd: 5
get content: new data

read later.
end thread receiving data on fd: 5
start new thread 126484089931328 to receive data on fd: 5
get content: hahaah

read later.
end thread receiving data on fd: 5
^C  # 结束主线程
```

