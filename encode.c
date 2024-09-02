/*
brainwire encoder

Copyright 2024 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>

int abs_sgn(int x)
{
	return (abs(x) << 1) | (x < 0);
}

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
	char wav_header[44];
	if (fread(wav_header, sizeof(wav_header), 1, input) != 1) {
		fprintf(stderr, "could not read 44 bytes from %s\n", argv[1]);
		return 1;
	}
	FILE *output = fopen(argv[2], "w");
	if (!output) {
		fprintf(stderr, "could not open %s for writing\n", argv[2]);
		return 1;
	}
	if (fwrite(wav_header, sizeof(wav_header), 1, output) != 1) {
		fprintf(stderr, "could not write 44 bytes to %s\n", argv[2]);
		return 1;
	}
	int prev = 0;
	short value;
	while (fread(&value, 2, 1, input) == 1) {
		unsigned short diff = abs_sgn(value - prev);
		prev = value;
		fwrite(&diff, 2, 1, output);
	}
	fclose(input);
	fclose(output);
	return 0;
}