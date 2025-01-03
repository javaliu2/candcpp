/**
 * 代码清单7-1 测试进程的UID和EUID的区别
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>

int main() {
    // Get the real user ID of the calling process.
    uid_t uid = getuid();
    // Get the effective user ID of the calling process.
    uid_t euid = geteuid();
    // user_id is 1000, effective user_id is 1000
    printf("user_id is %d, effective user_id is %d\n", uid, euid);
    return 0;
    int* a;
    sizeof(a); // 8
}