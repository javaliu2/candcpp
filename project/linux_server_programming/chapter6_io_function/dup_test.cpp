/**
 * dup()函数学习
 */
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main() {
    /**
     * open(filepath, operation_flag, permission)
     * operation_flag: 
        * 1) O_RDONLY: 只读
        * 2) O_WRONLY: 只写
        * 3) O_RDWR: 读写
        * 4) O_CREAT: 文件不存在则创建
        * 5) O_APPEND: 在文件末尾追加内容
        * 6) O_EXCL: 创建文件时确保其不存在，如果文件已经存在，返回错误
        * 7）O_TRUNC: truncation，截断，即将文件内容清空
     * permission:
        * 0: 表示这是一个八进制数
        * 6: 所有者权限，读写，rw-
        * 4: 组用户权限，只读，r--
        * 4: 其他用户权限，只读，r--
     */
    int a = 0100;  // 八进制
    int b = 0b0101; // 二进制
    int c = 0xea34;  // 十六进制
    int d = 100;  // 十进制

    int fd = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("failed to open file");
        return 1;
    }
    // dup2(fd, STDOUT_FILENO);
    // printf("this will go into the file instead of the screen!\n");
    int newfd = dup(fd);
    if (newfd == -1) {
        perror("failed to duplicate file descriptor");
        return 1;
    }
    int num = write(newfd, "Hello dup()", 12);
    if (num == -1) {
        perror("write failed");
        return 1;
    }
    printf("write %d bytes data\n", num);
    close(fd);
    close(newfd);
    return 0;
}