#include <stdio.h>
#include <unistd.h>

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid;

    // 创建两个管道
    pipe(pipe1); // pipe1[0]为读端，pipe1[1]为写端
    pipe(pipe2); // pipe2[0]为读端，pipe2[1]为写端

    pid = fork();  // 创建子进程
    // 父子进程从fork()后各自执行，共享两个管道变量（文件描述符）
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {  // 子进程
        close(pipe1[1]);  // 关闭pipe1的写端
        close(pipe2[0]);  // 关闭pipe2的读端

        char msg_from_parent[20];
        read(pipe1[0], msg_from_parent, sizeof(msg_from_parent)); // 从父进程读取数据
        printf("Child received: %s\n", msg_from_parent);

        write(pipe2[1], "Message from child", 18);  // 向父进程写数据
    } else {  // 父进程
        close(pipe1[0]);  // 关闭pipe1的读端
        close(pipe2[1]);  // 关闭pipe2的写端

        write(pipe1[1], "Message from parent", 19);  // 向子进程写数据

        char msg_from_child[20];
        read(pipe2[0], msg_from_child, sizeof(msg_from_child));  // 从子进程读取数据
        printf("Parent received: %s\n", msg_from_child);
    }

    return 0;
}
