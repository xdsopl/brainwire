/*
brainwire encoder

Copyright 2024 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>

int div_rnd(int n, int d) {
	if (n < 0)
		n -= d / 2;
	else if (n > 0)
		n += d / 2;
	return n / d;
}

int put_bit(FILE *file, int b) {
	static int acc, cnt;
	acc |= !!b << cnt++;
	if (cnt >= 8) {
		cnt -= 8;
		int tmp = acc & 255;
		acc >>= 8;
		return fputc(tmp, file) == EOF;
	}
	return 0;
}

int write_bits(FILE *file, int b, int n) {
	for (int i = 0; i < n; ++i) {
		int ret = put_bit(file, (b >> i) & 1);
		if (ret)
			return ret;
	}
	return 0;
}

int put_vli(FILE *file, int *order, int val)
{
	int ret;
	while (val >= 1 << *order) {
		if ((ret = put_bit(file, 0)))
			return ret;
		val -= 1 << *order;
		*order += 1;
	}
	if ((ret = put_bit(file, 1)))
		return ret;
	if ((ret = write_bits(file, val, *order)))
		return ret;
	*order -= 1;
	if (*order < 0)
		*order = 0;
	return 0;
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
	short value, prev = 0;
	int pred_order = 0, err_order = 0;
	while (fread(&value, 2, 1, input) == 1) {
		int diff = value - prev;
		prev = value;
		int pred = div_rnd(diff, 64);
		put_vli(output, &pred_order, abs(pred));
		int err = diff - pred * 64;
		put_vli(output, &err_order, abs(err));
		if (pred)
			put_bit(output, pred < 0);
		if (err)
			put_bit(output, err < 0);
	}
	int sentinel = 1024;
	put_vli(output, &pred_order, sentinel);
	put_vli(output, &pred_order, 255);
	fclose(input);
	fclose(output);
	return 0;
}