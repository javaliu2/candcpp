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

## Linux

#### 1、命令

chmod: change mode更改文件的模式

```sh
chmod +x filename  // 更改filename指定的文件模式为'可执行'
```

