/*
brainwire decoder

Copyright 2024 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>

int sgn_int(int x) {
	return (x & 1) ? -(x >> 1) : (x >> 1);
}

int get_bit(FILE *file) {
	static int cnt, acc;
	if (!cnt) {
		int b = fgetc(file);
		if (b < 0)
			return b;
		acc = b;
		cnt = 8;
	}
	int b = acc & 1;
	acc >>= 1;
	cnt -= 1;
	return b;
}

int read_bits(FILE *file, int *b, int n) {
	int a = 0;
	for (int i = 0; i < n; ++i) {
		int b = get_bit(file);
		if (b < 0)
			return b;
		a |= b << i;
	}
	*b = a;
	return 0;
}

int get_vli(FILE *file, int *order) {
	int val, sum = 0, ret;
	while ((ret = get_bit(file)) == 0) {
		sum += 1 << *order;
		*order += 1;
	}
	if (ret < 0)
		return ret;
	if ((ret = read_bits(file, &val, *order)))
		return ret;
	*order -= 1;
	if (*order < 0)
		*order = 0;
	return val + sum;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s input.brainwire output.wav\n", *argv);
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
	short value = 0;
	int pred, err, sentinel = 1 << 17;
	int pred_order = 0, err_order = 0;
	while ((pred = get_vli(input, &pred_order)) >= 0 && (err = get_vli(input, &err_order)) >= 0) {
		if (pred == sentinel)
			break;
		value += sgn_int(pred) * 64 + sgn_int(err);
		fwrite(&value, 2, 1, output);
	}
	fclose(input);
	fclose(output);
	return 0;
}