/**
 * 代码清单6-5 同时输出数据到终端和文件的程序
 * tee名字来源于T型管道，形象地表示了数据流在一个分支处分成两部分，用于不同的用途
 */
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("usage: %s <file>\n", argv[0]);
        return 1;
    }

    int filefd = open(argv[1], O_CREAT|O_WRONLY|O_TRUNC, 0666);
    assert(filefd > 0);

    int pipefd_stdout[2];
    int ret = pipe(pipefd_stdout);
    assert(ret != -1);

    int pipefd_file[2];
    ret = pipe(pipefd_file);
    assert(ret != -1);
    /** pipefd[0]为读端, 即输出; pipefd[1]为写端, 即输入.
        看站在谁的视角看待pipe的输入/输出.
        对于管道自己来说, 自己的输入端对别人来说是写端;
        自己的输出端, 对别人来说是读端.
    */
    // 将标准输入内容输入管道pipefd_stdout
    ret = splice(STDIN_FILENO, NULL, pipefd_stdout[1], NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
    assert(ret != -1);
    
    // 将管道pipefd_stdout的输出'复制'到管道pipefd_file的输入端
    ret = tee(pipefd_stdout[0], pipefd_file[1], 32768, SPLICE_F_NONBLOCK);
    // ret = splice(pipefd_stdout[0], NULL, pipefd_file[1], NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
    assert(ret != -1);

    // 将管道pipefd_file的输出定向到文件描述符filefd上，从而将标准输出内容写入文件
    ret = splice(pipefd_file[0], NULL, filefd, NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
    assert(ret != -1);

    // 将管道pipefd_stdout的输出定向到标准输出，其内容和写入文件的内容完全一致
    ret = splice(pipefd_stdout[0], NULL, STDOUT_FILENO, NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
    assert(ret != -1);

    close(filefd);
    close(pipefd_stdout[0]);
    close(pipefd_stdout[1]);
    close(pipefd_file[0]);
    close(pipefd_file[1]);
    printf("close all fd");
    return 0;
}