all: httpd client
LIBS = -lpthread #-lsocket
# 使用gcc编译器，后面是编译指令，-g: 生成调试信息
# -W: 启动所有警告; -Wall: 显示常见的警告信息
# $(LIBS): 链接变量LIBS指定的库
# -o $@: 输出文件名被替换为$@变量指定的值，这里是httpd,
# $<: 规则的第一个依赖项，即httpd.c
httpd: httpd.c
	gcc -g -W -Wall $(LIBS) -o $@ $<
client: simpleclient.c
	gcc -W -Wall -o $@ $<
clean:
	rm -f httpd client
