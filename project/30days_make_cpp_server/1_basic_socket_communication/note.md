```c
/* Structure describing an Internet socket address.  描述一个网络socket地址的结构体 */
struct sockaddr_in
  {
    __SOCKADDR_COMMON (sin_);  // 宏展开为sa_family_t sin_family，sin指的是socket Internet，
    // sa_family_t被定义为typedef unsigned short int sa_family_t，故其大小为2字节
    in_port_t sin_port;  // 端口号，typedef uint16_t in_port_t，大小2字节
    struct in_addr sin_addr; // 网络地址结构体，大小4字节
    /* Pad to size of `struct sockaddr'.  对结构体sockaddr大小的追加*/
    unsigned char sin_zero[sizeof (struct sockaddr)
			   - __SOCKADDR_COMMON_SIZE
			   - sizeof (in_port_t)
			   - sizeof (struct in_addr)];  // 16-2-2-4=8字节
  };
```

结构体**in_addr**:

```c
/* Internet address.  网络地址 */
typedef uint32_t in_addr_t;
struct in_addr
  {
	in_addr_t s_addr;
  };
```

结构体in_addr大小4字节。

结构体**sockaddr**:

```c
/* Structure describing a generic socket address.  描述一个通用socket地址的结构体 */
struct sockaddr
  {
    __SOCKADDR_COMMON (sa_);	/* Common data: address family and length.  公共数据: 地址协议族和长度
    展开宏: sa_family_t sa_family, sa表示socket address */
    char sa_data[14];		/* Address data.  地址数据 */
  };
```

由此可知结构体sockaddr的大小为16字节。