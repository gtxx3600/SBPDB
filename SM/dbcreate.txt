#include <stdio.h>
#include <unistd.h>

#include "rm.h"
#include "sm.h"

int main(int argc, char **argv) {
	char *dbname;
	char command[80] = "mkdir";

	if (argc != 2) {
		fprintf(stderr, "Usage: %s dbname\n", argv[0]);
	}
	dbname = argv[1];
	system(strcat(command, dbname));
	if (chdir(dbname) < 0) {
		fprintf(stderr, "chdir error to %s\n", dbname);
		exit(1);
	}

	return 0;
}
