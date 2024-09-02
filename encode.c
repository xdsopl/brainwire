/*
brainwire encoder

Copyright 2024 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s input.wav output.brainwire\n", *argv);
		return 1;
	}
	FILE *input = fopen(argv[1], "r");
	if (!input) {
		fprintf(stderr, "could not open %s for reading\n", argv[1]);
		return 1;
	}
	FILE *output = fopen(argv[2], "w");
	if (!output) {
		fprintf(stderr, "could not open %s for writing\n", argv[2]);
		return 1;
	}
	short value;
	while (fread(&value, 2, 1, input) == 1)
		fwrite(&value, 2, 1, output);
	fclose(input);
	fclose(output);
	return 0;
}