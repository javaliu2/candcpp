#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

pid_t gettid() {
	return syscall(SYS_gettid);
}

void *start_routine(void *arg) {
	pid_t pid = gettid();
	pthread_t tid = pthread_self();
	printf("thd%d: pid=%d, tid=%lu\n", *((int *)arg), pid, tid);

	char msg[32] = "";
	snprintf(msg, sizeof(msg)-1, "thd%d: i am thd%d\n", *((int *)arg), *((int *)arg));
	while (1) {
		write(1, msg, strlen(msg));
		sleep(1);
	}
}

int main() {

	pid_t pid = gettid();
	pthread_t tid = pthread_self();
	printf("main: pid=%d, tid=%lu\n", pid, tid);

	int th1 = 1;
	pthread_t tid1;
	pthread_create(&tid1, NULL, start_routine, &th1);

	int th2 = 2;
	pthread_t tid2;
	pthread_create(&tid2, NULL, start_routine, &th2);
	
	int th3 = 3;
	pthread_t tid3;
	pthread_create(&tid3, NULL, start_routine, &th3);

	const char *msg = "main: i am main\n";
	while (1) {
		write(1, msg, strlen(msg));
		sleep(1);
	}

	return 0;
}
