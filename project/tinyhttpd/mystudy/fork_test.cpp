#include <stdio.h>
#include <unistd.h>

int main() {
    /**
     * Clone the calling process, creating an exact copy.
       Return -1 for errors, 0 to the new process,
       and the process ID of the new process to the old process.
       克隆调用fork()的进程，创建一个精确拷贝（包括内存数据、堆栈、程序计数器、打开的文件描述符等）
       错误返回-1，对于子进程来说返回0，对于创建子进程的父进程来说返回子进程的PID
     */
    pid_t pid = fork();  // 创建子进程

    if (pid < 0) {
        // fork() 调用失败
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // 这是子进程
        printf("Child process: pid = %d\n", getpid());  // Get the process ID of the calling process.
    } else {
        // 这是父进程
        printf("Parent process: pid = %d, child pid = %d\n", getpid(), pid);
    }
    /**
     * OUTPUT:
     * Parent process: pid = 141669, child pid = 141670
     * Child process: pid = 141670
     * 因为两个进程程序计数器一样，因此都是从line 12之后开始执行
     * 父进程的PID==子进程的PID，子进程的PID==0，所以会有两行输出
     */
    return 0;
}
