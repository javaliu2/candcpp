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

## 9.8 超级服务xinetd

x表示超级的意思，inet，互联网服务，d是单词daemon，即守护进程。

```shell
ps aux | grep xinetd
USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
xs         15060  0.0  0.0  16236  4600 pts/5    S+   11:06   0:00 grep --color=auto xinetd
# VSZ: 进程使用的虚拟内存大小(KB)
# RSS: 进程使用的常驻内存大小(KB)
# TTY: 进程关联的终端（如果有的话）
# STAT: 进程状态（例如，R表示运行，S表示休眠）
# START: 进程的启动时间
# TIME: 进程占用的CPU时间
```

在 `ps` 命令中，`aux` 是常用的选项组合，用来显示系统中所有进程的信息。每个字母代表不同的含义：

- **`a`**: 显示所有用户的进程，包括其他用户的进程（即非当前终端的进程）。
- **`u`**: 显示进程的所属用户（用户列），即进程由哪个用户启动的。
- **`x`**: 显示所有进程，包括没有控制终端的进程（例如守护进程和后台进程）。

### 组合解释：

- **`ps aux`**：会显示系统上所有进程的信息，包括每个进程的用户、PID（进程 ID）、CPU 使用率、内存使用率、启动时间、运行时间、命令等。

```shell
xs@xslab:~$ cat /var/run/xinetd.pid
15858
xs@xslab:~$ ps -eo pid,ppid,pgid,sid,comm|grep 15858
  15858       1   15858   15858 xinetd
xs@xslab:~$ ps -eo pid,ppid,pgid,sid,comm|grep 15858
  15858       1   15858   15858 xinetd
  17568   15858   17568   17568 in.telnetd
xs@xslab:~$ sudo lsof -p 17568
lsof: WARNING: can't stat() fuse.portal file system /run/user/1000/doc
      Output information may be incomplete.
COMMAND     PID    USER   FD   TYPE DEVICE SIZE/OFF  NODE NAME
in.telnet 17568 telnetd  cwd    DIR  259,2     4096     2 /
in.telnet 17568 telnetd  rtd    DIR  259,2     4096     2 /
in.telnet 17568 telnetd  txt    REG  259,2    56000 79491 /usr/sbin/in.telnetd
in.telnet 17568 telnetd  mem    REG  259,2    18664 11125 /usr/lib/x86_64-linux-gnu/libnss_mdns4_minimal.so.2
in.telnet 17568 telnetd  mem    REG  259,2  2220400  5995 /usr/lib/x86_64-linux-gnu/libc.so.6
in.telnet 17568 telnetd  mem    REG  259,2   240936  5964 /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
in.telnet 17568 telnetd    0u  IPv4 299686      0t0   TCP localhost:telnet->localhost:35250 (ESTABLISHED)
in.telnet 17568 telnetd    1u  IPv4 299686      0t0   TCP localhost:telnet->localhost:35250 (ESTABLISHED)
in.telnet 17568 telnetd    2u  IPv4 299686      0t0   TCP localhost:telnet->localhost:35250 (ESTABLISHED)
in.telnet 17568 telnetd    3u   CHR    5,2      0t0    87 /dev/ptmx

```

### G-bro summary:

在 Linux 和类 Unix 系统中，进程管理非常重要，理解进程的各个标识符对系统管理和调试非常有帮助。你提到的 `pid`, `ppid`, `pgid`, `sid`, `comm` 都是与进程相关的标识符。以下是这些字段的详细说明：

#### 1. **PID (Process ID)**

- **全称**: Process ID（进程标识符）
- **描述**: 每个正在运行的进程都有一个唯一的进程 ID（PID），它是操作系统用来识别进程的数字标识。
- **作用**: 通过 PID，操作系统可以区分不同的进程。每当启动一个新进程时，操作系统分配一个唯一的 PID。

**示例**：

```bash
ps aux | grep <进程名>
```

输出中会显示进程的 PID。

#### 2. **PPID (Parent Process ID)**

- **全称**: Parent Process ID（父进程标识符）
- **描述**: 父进程的 PID，表示当前进程是由哪个进程创建的。每个进程除了初始化进程（PID 为 1）外，都会有一个父进程。
- **作用**: 通过 PPID，操作系统能够建立进程间的层级关系，了解哪个进程创建了当前进程。

**示例**：

```bash
ps -ef
```

其中，`PPID` 显示了父进程的 PID。

#### 3. **PGID (Process Group ID)**

- **全称**: Process Group ID（进程组标识符）
- **描述**: 进程组 ID 是一组相关进程的标识符，通常用于将一组进程当作一个单元来管理。在 Unix 和 Linux 系统中，一组进程可以共同组成一个进程组，进程组内的所有进程共享同一个 PGID。
- **作用**: PGID 常用于作业控制和信号传递等场景。例如，发送信号给整个进程组（而不仅仅是单个进程）。

**示例**：

```bash
ps -eo pid,pgid,comm
```

输出会显示每个进程的 PID 和 PGID。

#### 4. **SID (Session ID)**

- **全称**: Session ID（会话标识符）
- **描述**: 会话 ID 是与一组进程相关的标识符。会话通常包含一个或多个进程组，通常用于管理终端会话。每个会话由一个会话领导进程（通常是该会话中的第一个进程）发起。
- **作用**: SID 用于管理和控制会话中的进程。例如，在 Linux 中，当你登录时，登录进程会成为会话的领导进程，其他进程（如 shell、应用程序等）都将属于同一个会话。

**示例**：

```bash
ps -eo pid,sid,comm
```

输出会显示每个进程的 PID 和 SID。

#### 5. **COMM (Command Name)**

- **全称**: Command Name（命令名）
- **描述**: 进程的命令名称，即启动该进程时的可执行文件名。它是进程所执行的程序的名称，不包括路径。
- **作用**: `comm` 字段表示进程的可执行文件名。它通常用于识别运行的程序。

**示例**：

```bash
ps -eo pid,comm
```

输出会显示每个进程的 PID 和命令名称。

------

#### 总结：

- **PID**：进程的唯一标识符。
- **PPID**：父进程的 PID，表示当前进程是由哪个进程创建的。
- **PGID**：进程组的标识符，表示一组相关进程。
- **SID**：会话的标识符，表示与终端会话相关的进程。
- **COMM**：进程的命令名称，表示启动该进程时的可执行文件名。

这些标识符在进程管理和调试中非常有用，可以帮助你理解进程的层级关系、控制进程组、会话等。
