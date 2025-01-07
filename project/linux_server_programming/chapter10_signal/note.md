# 第十章 信号

## 10.5 网络编程相关信号

### 10.5.1 SIGHUP

1、开启xinetd上的echo-stream服务，设置/etc/xinetd.d/echo文件中disable为no，开启tcp版本的echo.

```shell
# default: off
# description: An xinetd internal service which echo's characters back to
# clients.
# This is the tcp version.
service echo
{
	disable		= no
	type		= INTERNAL
	id		= echo-stream
	socket_type	= stream
	protocol	= tcp
	user		= root
	wait		= no
}

# This is the udp version.
service echo
{
	disable		= yes
	type		= INTERNAL
	id		= echo-dgram
	socket_type	= dgram
	protocol	= udp
	user		= root
	wait		= yes
}
```

2、重启xinetd服务命令

```shell
sudo systemctl restart xinetd
```

3、telnet localhost:7 上的echo服务

```shell
(base) xs@xslab:~$ telnet 127.0.0.1 7
Trying 127.0.0.1...
Connected to 127.0.0.1.
Escape character is '^]'.
ddd
ddd
echo hello
echo hello

```

4、查看xinetd进程

```shell
(base) xs@xslab:~$ ps -ef | grep inetd
root        9471       1  0 09:23 ?        00:00:00 /usr/sbin/xinetd -pidfile /run/xinetd.pid -stayalive -inetd_compat -inetd_ipv6
root       10763    9471  0 09:30 ?        00:00:00 (xinetd service)  echo-stream localhost                                      
xs         10765    8652  0 09:30 pts/1    00:00:00 grep --color=auto inetd
```

-ef: e表示everyone，所有用户；f表示full，即显示进程的详细信息，包括UID，PID，GID，PPID等待。

由以上可知，9471是xinetd进程的ID。

5、查看xinetd进程打开文件的信息

```shell
(base) xs@xslab:~$ sudo lsof -p 9471
lsof: WARNING: can't stat() fuse.portal file system /run/user/1000/doc
      Output information may be incomplete.
COMMAND  PID USER   FD   TYPE             DEVICE SIZE/OFF    NODE NAME
xinetd  9471 root  cwd    DIR              259,2     4096       2 /
xinetd  9471 root  rtd    DIR              259,2     4096       2 /
xinetd  9471 root  txt    REG              259,2   174496 1253377 /usr/sbin/xinetd
xinetd  9471 root  mem    REG              259,2    68552    6056 /usr/lib/x86_64-linux-gnu/libresolv.so.2
xinetd  9471 root  mem    REG              259,2    22600    4393 /usr/lib/x86_64-linux-gnu/libkeyutils.so.1.9
xinetd  9471 root  mem    REG              259,2    52016    4818 /usr/lib/x86_64-linux-gnu/libkrb5support.so.0.1
xinetd  9471 root  mem    REG              259,2    18504    4113 /usr/lib/x86_64-linux-gnu/libcom_err.so.2.1
xinetd  9471 root  mem    REG              259,2   182864    3986 /usr/lib/x86_64-linux-gnu/libk5crypto.so.3.1
xinetd  9471 root  mem    REG              259,2   827936    4131 /usr/lib/x86_64-linux-gnu/libkrb5.so.3.3
xinetd  9471 root  mem    REG              259,2   338648    4048 /usr/lib/x86_64-linux-gnu/libgssapi_krb5.so.2.2
xinetd  9471 root  mem    REG              259,2   182912    7229 /usr/lib/x86_64-linux-gnu/libtirpc.so.3.0.0
xinetd  9471 root  mem    REG              259,2   613064    4501 /usr/lib/x86_64-linux-gnu/libpcre2-8.so.0.10.4
xinetd  9471 root  mem    REG              259,2    93280    7240 /usr/lib/x86_64-linux-gnu/libnsl.so.2.0.1
xinetd  9471 root  mem    REG              259,2  2220400    5995 /usr/lib/x86_64-linux-gnu/libc.so.6
xinetd  9471 root  mem    REG              259,2   166280    5566 /usr/lib/x86_64-linux-gnu/libselinux.so.1
xinetd  9471 root  mem    REG              259,2    44872    9823 /usr/lib/x86_64-linux-gnu/libwrap.so.0.7.6
xinetd  9471 root  mem    REG              259,2   240936    5964 /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
xinetd  9471 root    0r   CHR                1,3      0t0       5 /dev/null
xinetd  9471 root    1r   CHR                1,3      0t0       5 /dev/null
xinetd  9471 root    2r   CHR                1,3      0t0       5 /dev/null
xinetd  9471 root    3r  FIFO               0,13      0t0  158911 pipe
xinetd  9471 root    4w  FIFO               0,13      0t0  158911 pipe
xinetd  9471 root    5u  IPv6             156093      0t0     TCP *:echo (LISTEN)
xinetd  9471 root    6u  IPv6             156094      0t0     TCP *:telnet (LISTEN)
xinetd  9471 root    7u  unix 0xffff92ac451a1dc0      0t0  156088 type=DGRAM
```

TYPE：DIR：目录；REG：常规文件；CHR：字符设备；FIFO：管道；IPv6/IPv4：网络socket.

0r：read；4w：write；5u：read和write.

可知xinetd打开一个pipe，fd 3用来写，fd 4用来度，同时xinetd负责监听echo和telnet子服务。

6、打开time服务，追踪xinetd的变化

// 尝试关闭\打开echo服务，strace追踪到的内容没有创建socket，echo服务也不能启动起来，不知道怎么回事。所以尝试启动time服务

1）修改/etc/xinetd.d/time中tcp版本的disable为no，即非禁用，即启用。

2）终端追踪xinetd的切换变化，将过程写入a文件，'&>'表示标准输出和标准错误一起写入。

```shell
(base) xs@xslab:~$ sudo strace -p 14501 &> a
^C
```

3）检查xinetd打开的fd情况

```shell
(base) xs@xslab:~$ sudo lsof -p 14501
lsof: WARNING: can't stat() fuse.portal file system /run/user/1000/doc
      Output information may be incomplete.
COMMAND   PID USER   FD   TYPE             DEVICE SIZE/OFF    NODE NAME
xinetd  14501 root  cwd    DIR              259,2     4096       2 /
xinetd  14501 root  rtd    DIR              259,2     4096       2 /
xinetd  14501 root  txt    REG              259,2   174496 1253377 /usr/sbin/xinetd
xinetd  14501 root  mem    REG              259,2    68552    6056 /usr/lib/x86_64-linux-gnu/libresolv.so.2
xinetd  14501 root  mem    REG              259,2    22600    4393 /usr/lib/x86_64-linux-gnu/libkeyutils.so.1.9
xinetd  14501 root  mem    REG              259,2    52016    4818 /usr/lib/x86_64-linux-gnu/libkrb5support.so.0.1
xinetd  14501 root  mem    REG              259,2    18504    4113 /usr/lib/x86_64-linux-gnu/libcom_err.so.2.1
xinetd  14501 root  mem    REG              259,2   182864    3986 /usr/lib/x86_64-linux-gnu/libk5crypto.so.3.1
xinetd  14501 root  mem    REG              259,2   827936    4131 /usr/lib/x86_64-linux-gnu/libkrb5.so.3.3
xinetd  14501 root  mem    REG              259,2   338648    4048 /usr/lib/x86_64-linux-gnu/libgssapi_krb5.so.2.2
xinetd  14501 root  mem    REG              259,2   182912    7229 /usr/lib/x86_64-linux-gnu/libtirpc.so.3.0.0
xinetd  14501 root  mem    REG              259,2   613064    4501 /usr/lib/x86_64-linux-gnu/libpcre2-8.so.0.10.4
xinetd  14501 root  mem    REG              259,2    93280    7240 /usr/lib/x86_64-linux-gnu/libnsl.so.2.0.1
xinetd  14501 root  mem    REG              259,2  2220400    5995 /usr/lib/x86_64-linux-gnu/libc.so.6
xinetd  14501 root  mem    REG              259,2   166280    5566 /usr/lib/x86_64-linux-gnu/libselinux.so.1
xinetd  14501 root  mem    REG              259,2    44872    9823 /usr/lib/x86_64-linux-gnu/libwrap.so.0.7.6
xinetd  14501 root  mem    REG              259,2   240936    5964 /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
xinetd  14501 root    0r   CHR                1,3      0t0       5 /dev/null
xinetd  14501 root    1r   CHR                1,3      0t0       5 /dev/null
xinetd  14501 root    2r   CHR                1,3      0t0       5 /dev/null
xinetd  14501 root    3r  FIFO               0,13      0t0  386932 pipe
xinetd  14501 root    4w  FIFO               0,13      0t0  386932 pipe
xinetd  14501 root    5u  IPv6             389602      0t0     TCP *:telnet (LISTEN)
xinetd  14501 root    6u  IPv6             394264      0t0     TCP *:time (LISTEN)
xinetd  14501 root    7u  unix 0xffff92adc07d7b40      0t0  389597 type=DGRAM

```

发现自增一个fd 6，即time服务，可读可写。

4）检查a文件内容，仅列出一部分

```shell
strace: Process 14501 attached  # 执行 sudo strace -p 14501 &> a 命令导致strace记录了这行日志
restart_syscall(<... resuming interrupted read ...>) = ? ERESTART_RESTARTBLOCK (Interrupted by signal)
--- SIGHUP {si_signo=SIGHUP, si_code=SI_USER, si_pid=14663, si_uid=0} ---
write(4, "\1", 1)                       = 1  # "\1"中的1就是SIGHUP，信号处理函数使用管道通知主程序该信号，4为管道写端
rt_sigreturn({mask=[]})                 = -1 EINTR (Interrupted system call)
poll([{fd=5, events=POLLIN}, {fd=3, events=POLLIN}], 2, -1) = 1 ([{fd=3, revents=POLLIN}]) # 主程序检测到fd 3，即管道读端上有可读事件
ioctl(3, FIONREAD, [1])                 = 0
read(3, "\1", 1)                        = 1  # 主程序读取该事件
getpid()                                = 14501
sendto(7, "<29>Jan  7 11:02:41 xinetd[14501"..., 59, MSG_NOSIGNAL, NULL, 0) = 59  # 通过fd 7，即本地socket发送消息，udp读写函数
openat(AT_FDCWD, "/etc/xinetd.conf", O_RDONLY) = 6
fstat(6, {st_mode=S_IFREG|0644, st_size=289, ...}) = 0
lseek(6, 0, SEEK_CUR)                   = 0
fcntl(6, F_GETFL)                       = 0x8000 (flags O_RDONLY|O_LARGEFILE)
read(6, "# Simple configuration file for "..., 8192) = 289
openat(AT_FDCWD, "/etc/xinetd.d", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 8
newfstatat(8, "", {st_mode=S_IFDIR|0755, st_size=4096, ...}, AT_EMPTY_PATH) = 0
getdents64(8, 0x55a7a11adda0 /* 14 entries */, 32768) = 416
getdents64(8, 0x55a7a11adda0 /* 0 entries */, 32768) = 0
close(8)                                = 0
...
stat("/etc/xinetd.d/time", {st_mode=S_IFREG|0644, st_size=568, ...}) = 0
openat(AT_FDCWD, "/etc/xinetd.d/time", O_RDONLY) = 8  # 读取time文件
getpid()                                = 14501
sendto(7, "<31>Jan  7 11:02:41 xinetd[14501"..., 126, MSG_NOSIGNAL, NULL, 0) = 126
fstat(8, {st_mode=S_IFREG|0644, st_size=568, ...}) = 0
lseek(8, 0, SEEK_CUR)                   = 0
fcntl(8, F_GETFL)                       = 0x8000 (flags O_RDONLY|O_LARGEFILE)
read(8, "# default: off\n# description: An"..., 8192) = 568
newfstatat(AT_FDCWD, "/etc/nsswitch.conf", {st_mode=S_IFREG|0644, st_size=582, ...}, 0) = 0
openat(AT_FDCWD, "/etc/protocols", O_RDONLY|O_CLOEXEC) = 9
newfstatat(9, "", {st_mode=S_IFREG|0644, st_size=2932, ...}, AT_EMPTY_PATH) = 0
lseek(9, 0, SEEK_SET)                   = 0
read(9, "# Internet (IP) protocols\n#\n# Up"..., 4096) = 2932
close(9)                                = 0
newfstatat(AT_FDCWD, "/etc/nsswitch.conf", {st_mode=S_IFREG|0644, st_size=582, ...}, 0) = 0
openat(AT_FDCWD, "/etc/passwd", O_RDONLY|O_CLOEXEC) = 9
newfstatat(9, "", {st_mode=S_IFREG|0644, st_size=3446, ...}, AT_EMPTY_PATH) = 0
lseek(9, 0, SEEK_SET)                   = 0
read(9, "root:x:0:0:root:/root:/bin/bash\n"..., 4096) = 3446
close(9)                                = 0
read(8, "", 8192)                       = 0
close(8)                                = 0
...
socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP) = 6  # 创建流socket，返回的fd为6
fcntl(6, F_SETFD, FD_CLOEXEC)           = 0
setsockopt(6, SOL_IPV6, IPV6_V6ONLY, [0], 4) = 0 # 4 是第三个optval的size
setsockopt(6, SOL_SOCKET, SO_REUSEADDR, [1], 4) = 0
bind(6, {sa_family=AF_INET6, sin6_port=htons(37), sin6_flowinfo=htonl(0), inet_pton(AF_INET6, "::", &sin6_addr), sin6_scope_id=0}, 28) = 0  # ip_address: localhost, 端口37
listen(6, 64)                           = 0
getpid()                                = 14501
sendto(7, "<29>Jan  7 11:02:41 xinetd[14501"..., 81, MSG_NOSIGNAL, NULL, 0) = 81
poll([{fd=5, events=POLLIN}, {fd=3, events=POLLIN}, {fd=6, events=POLLIN}], 3, -1strace: Process 14501 detached
 <detached ...>  # 命令行^C导致strace退出
```

