#include "main.h"

FILE *logfile = NULL;
int line = 0;

void *Malloc(size_t len) {
	void *toreturn;
	if((toreturn = malloc(len)) == NULL) {
		error("malloc failed");
	}
	return toreturn;
}

void inc_line() {
	++line;
}

int get_next_line(char *buf, size_t size, FILE **file) {
	FILE *f = *file;
	if(fgets(buf, size, f) == NULL)
		return -1;

	if(strlen(buf) >= size*3/4) {
		error("Too long of a line detected... bummer");
		return -2;
	}
	return 0;
}

void error(char const *msg) {
	fprintf(stderr, "Error: %s, aborting\n", msg);
	exit(-1);
	return;
}

long Strtol(char const *nptr, char **endptr, int base) {
	return atoi(nptr);
}

int open_log(char const *filename) {
	if(logfile != NULL)
		return -1;
	logfile = fopen(filename, "w");
	
	if(logfile == NULL)
		return -1;
	else
		return 0;
}

void logger(char const *msg) {
	if(logfile == NULL)
		fprintf(stderr, "%d: %s\n", line, msg);
	else
		fprintf(logfile, "%d: %s\n", line, msg);
}
