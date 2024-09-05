/*
brainwire encoder

Copyright 2024 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int uns_int(int x) {
	return (x << 1) ^ (x >> 31);
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

int put_rice(FILE *file, int x, int k) {
	int ret;
	for (int q = x >> k; q; --q)
		if ((ret = put_bit(file, 0)))
			return ret;
	if ((ret = put_bit(file, 1)))
		return ret;
	if ((ret = write_bits(file, x & ((1 << k) - 1), k)))
		return ret;
	return 0;
}

int recon(int x) {
	// mapping found by Dominic Szablewski
	return x * 64.061577 + (x < 0 ? 31.527393 : 31.534184);
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
	short value;
	int prev = 0, k = 3;
	while (fread(&value, 2, 1, input) == 1) {
		int quant = (value + 32768) / 64 - 512;
		assert(value == recon(quant));
		int diff = quant - prev;
		prev = quant;
		put_rice(output, uns_int(diff), k);
	}
	int sentinel = 1024;
	put_rice(output, sentinel, k);
	put_rice(output, 255, k);
	int ratio = (ftell(input) * 100) / ftell(output);
	fprintf(stderr, "compression ratio = %d%%\n", ratio);
	fclose(input);
	fclose(output);
	return 0;
}