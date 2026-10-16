#include <stdio.h>
#include <unistd.h>

void 
one_char_output(char *str) {
    if (*str != '\0') {
	// fork
        one_char_output(str + 1);  
        putchar(*str);            
    }
}

int
main(int argc, char** argv) {
	
	if (argc != 2) {
		printf("Arg error! Please pass a single word to the program");
		return -1;
	}

	char* inp;

	inp = argv[argc - 1];

	one_char_output(inp);
}
