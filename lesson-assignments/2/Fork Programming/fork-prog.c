#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

void
q1() {
	int x;

	x = 100;

	pid_t pid = fork();

	if (pid == 0) {
		x = 12;
		printf("This is a child process! here is x: %d \n", x);
	}
	else if (pid > 0) {
		x = 59;
		printf("This is the parent! here is x: %d\n", x);
	}
	else {
		printf("you don goofed \n");
	}
}

void
q2() {
	int fd;

	fd = open("answer.txt", O_RDWR | O_APPEND);

	if (fd == -1) {
		printf("Open error");
		return;
	}

	pid_t pid = fork();


	if (pid == 0) {

		printf(" \n Child \n");

		char buf[100];

		read(fd, buf, sizeof(buf));

		int i;

		for (i = 0 ; i < 50 ; i++ ) {

			putchar(buf[i]);

		}

		char* inp = " \n Hello from child! \n";

		write(fd, inp, strlen(inp));

	}
	else if (pid > 0) {

		printf(" \n Parent \n");

		char buf[100];

		ssize_t n = read(fd, buf, sizeof(buf));

		int i;

		for (i = 0 ; i < 50 ; i++ ) {
			putchar(buf[i]);
		}

		char* inp = "Hello from parent! \n";

		write(fd, inp, strlen(inp));

	}
	else {
		printf("you don goofed \n");
	}

}

void
q3() {
	// achiavble with a lock guard liek design maybe atomic
	int lock = 1; // locked

	int* lock_guard = &lock;

	pid_t pid = fork();
	

	if (pid == 0) {
		puts("hello");
		lock_guard = 0; // unlock
	}
	else if (pid > 0) {
		while (lock_guard) {}
		puts("goodbye");
	}

}

void
q4(int arg, char** envp) {

	char* to_run = "usr/ls/";

	switch (arg) {
		case 1:
			execve(to_run, NULL, envp);
		case 2:
			execle(to_run, NULL, envp);
		case 3:
			execl(to_run, NULL);
		case 4:
			execv(to_run, NULL);
		case 5:
			execvp(to_run, NULL);
		case 6:
			execlp(to_run, NULL);

	}

}

void
q5() {

	pid_t pid = fork();
	int* status;

	if (pid == 0) {

		// part 2
		pid_t ret = wait(status);
		if (ret == -1) {
			puts("No child to wiat for");
		}

		puts("hello");
	}
	else if (pid > 0) {

		// part 1
		// pid_t ret = wait(status);
		// if (ret == -1) {
		// 	puts("No child to wiat for");
		// }

		puts("goodbye");
	}

}

int 
main(int argc, char** argv, char** envp) {

	q5();

}
