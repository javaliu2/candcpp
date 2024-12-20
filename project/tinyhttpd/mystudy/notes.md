## 函数

### 1、startup

#### 1）函数签名

int startup(uint8_t *port);

接收指向端口号的指针，返回socket

#### 2）函数功能

在port指定的端口上开启监听web连接的进程

#### 3）具体实现

```c
int startup(uint8_t *port) {
    int httpd = 0;
    int on = 1;
    struct sockaddr_in name;

    httpd = socket(PF_INET, SOCK_STREAM, 0);
    if (httpd == -1) error_die("socket");
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
        error_die("setsockopt failed");
    }
    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
        error_die("bind");
    if (*port == 0) /* if dynamically allocating a port */
    {
        socklen_t namelen = sizeof(name);
        if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
            error_die("getsockname");
        *port = ntohs(name.sin_port);
    }
    if (listen(httpd, 5) < 0) error_die("listen");
    return (httpd);
}
```

#### 4）结构体\宏定义

```c
/* Structure describing an Internet socket address.  */
struct sockaddr_in
  {
    __SOCKADDR_COMMON (sin_);  // expands to sa_family_t sin_family, 2bytes
    in_port_t sin_port;			/* Port number.  2bytes*/
    struct in_addr sin_addr;		/* Internet address.  4bytes */

    /* Pad to size of `struct sockaddr'.  */
    unsigned char sin_zero[sizeof (struct sockaddr)   // 16bytes
			   - __SOCKADDR_COMMON_SIZE
			   - sizeof (in_port_t)
			   - sizeof (struct in_addr)];
  };
```

其中，宏 __SOCKADDR_COMMON 定义为：

```c
#define	__SOCKADDR_COMMON(sa_prefix) \
  sa_family_t sa_prefix##family
```

struct sockaddr 定义为：

```C
/* Structure describing a generic socket address.  */
struct sockaddr
  {
    __SOCKADDR_COMMON (sa_);	/* Common data: address family and length.  */
    char sa_data[14];		/* Address data.  */
  };
```

in_addr 定义为：

```c
/* Internet address.  */
typedef uint32_t in_addr_t;
struct in_addr
  {
    in_addr_t s_addr;
  };
```

### 2、accept_request

#### 1）函数签名

void accept_request(void *arg);

#### 2）情景分析

启动httpd程序，在4000端口监听客户端连接

浏览器请求：http://127.0.0.1:4000/

get_line()获取到请求的HTTP报文的第一行：GET / HTTP/1.1

前端提交"yellow"，HTTP报文第一行：POST /color.cgi HTTP/1.1

#### 3）请求不存在的资源

**URL:** http://127.0.0.1:4000/f

**HTTP报文头部：**

GET line 1: GET /f HTTP/1.1
GET line 2: Host: 127.0.0.1:4000
GET line 3: User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:133.0) Gecko/20100101 Firefox/133.0
GET line 4: Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
GET line 5: Accept-Language: en-US,en;q=0.5
GET line 6: Accept-Encoding: gzip, deflate, br, zstd
GET line 7: Connection: keep-alive
GET line 8: Upgrade-Insecure-Requests: 1
GET line 9: Sec-Fetch-Dest: document
GET line 10: Sec-Fetch-Mode: navigate
GET line 11: Sec-Fetch-Site: none
GET line 12: Sec-Fetch-User: ?1
GET line 13: Priority: u=0, i
GET line 14:

#### 4）提交颜色

输入'yellow'，点击提交，HTTP请求报文：

Content-Length: 12

表示HTTP报文payload数据大小为12个字节，原始数据是color=yellow，刚好12个字符，一个字符占用一个字节

同理，当输入'blue'提交时，Content-Length为10个字节。

## Linux

#### 1、命令

chmod: change mode更改文件的模式

```sh
chmod +x filename  // 更改filename指定的文件模式为'可执行'
```

### Architecture

![tinyhttpd-architecture](/home/xs/basic_skill/candcpp/project/tinyhttpd/mystudy/tinyhttpd-architecture.png)

注意，父进程和系统内核之间的两个管道和子进程和系统内核之间的两个管道是相同的对象，这样才可以保证父子进程之间的通信。同时，重定向系统（主机）的标准输入、输出为子进程的读、写端口。图上打×是表明关闭这些端口的读/写，因为针对具体的进程而言，不需要使用这些端口。
