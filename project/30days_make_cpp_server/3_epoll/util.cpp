#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
void errif(bool conditon, const char* errmsg) {
    if (conditon) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("%s: 暂时没有可处理的资源\n", errmsg);
        } else {
            /* Print a message describing the meaning of the value of errno.
             打印描述errno值意义的一条消息。
            */
            perror(errmsg);  // 打印 "<errmsg>: <errno 对应的错误信息>"
            exit(EXIT_FAILURE);
        }
    }
}