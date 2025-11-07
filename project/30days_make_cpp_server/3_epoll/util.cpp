#include "util.h"
#include <stdio.h>
#include <stdlib.h>
void errif(bool conditon, const char* errmsg) {
    if (conditon) {
        /* Print a message describing the meaning of the value of errno.
         打印描述errno值意义的一条消息。
        */
        perror(errmsg);  // 打印 "<errmsg>: <errno 对应的错误信息>"
        exit(EXIT_FAILURE);
    }
}