#include <stdio.h>
#include <pthread.h>

void
one_char_output(char c) {
	putchar(c);
}

void *
pthread_fork(void *str) {

   char c = *(char*) str;

    if (c != '\0') {
	// fork
        pthread_fork(str + 1);  
        one_char_output(c);            
    }
	return NULL;
}

int
main(int argc, char** argv) {

	if (argc != 2) {
		printf("Arg error! Please pass a single word to the program");
		return -1;
	}

	char* inp;

	inp = argv[argc - 1];

	pthread_t t;

	pthread_create(&t, NULL, pthread_fork, inp);
	void* ret;
	pthread_join(t, &ret);

	return 0;
}
