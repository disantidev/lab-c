#include <stdio.h>
#include <fcntl.h>

#define BUFFER_MAX_SIZE 64

int main (int argc, char *argv[])
{
	FILE *source_file, *destination_file;
	char buffer[BUFFER_MAX_SIZE];
	int buffer_size = 0;

	if (argv[1] == NULL) {
		perror("source was not provided");
		return 1;
	}

	if (argv[2] == NULL) {
		perror("destination was not provided");
		return 1;
	}

	if ((source_file = fopen(argv[1], "r")) == NULL) {
		perror("error on open source file");
		return 1;
	}

	if ((destination_file = fopen(argv[2], "w+")) == NULL) {
		perror("error on open destination file");
		return 1;
	}

	while ((buffer_size = fread(buffer, sizeof(char), BUFFER_MAX_SIZE, source_file)) > 0) {
		if ((fwrite(buffer, sizeof(char), buffer_size, destination_file)) != buffer_size) {
			perror("error on copy to destination file");
			return 1;
		}
	}

	fclose(source_file);
	fclose(destination_file);

	return 0;
}
