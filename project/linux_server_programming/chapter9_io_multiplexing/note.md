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
