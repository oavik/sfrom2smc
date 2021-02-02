
/*
Copyright (c) 2021 Oavik <oavik@oavik.net>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* see https://wiki.superfamicom.org/sfrom-file-format */
#define ROM_START_OFFSET	0x08
#define FOOTER_START_OFFSET	0x14
#define FOOTER_ROM_SIZE_OFFSET	0x01

static uint32_t read_uint32(FILE *in, size_t offset)
{
	if (fseek(in, offset, SEEK_SET) != 0) {
		perror("error seeking input file");
		exit(EXIT_FAILURE);
	}

	uint32_t r = 0;
	for (size_t i = 0; i < sizeof(uint32_t); i++) {
		int c = fgetc(in);
		if (c == EOF) {
			perror("error reading input");
			exit(EXIT_FAILURE);
		}
		r += c << (CHAR_BIT * i);
	}
	return r;
}

int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "usage: %s input.sfrom [output.smc]\n", argv[0]);
		return 1;
	}

	char *input = argv[1];
	char *output = NULL;
	if (argc == 3) {
		output = argv[2];
	} else {
		output = malloc(strlen(input) + 1);
		if (output == NULL) {
			perror("error allocating memory for output file name");
		}
		strcpy(output, input);
		char *dot = strstr(output, ".sfrom");
		if (!dot) {
			fprintf(stderr, "input file missing .sfrom extension\n");
			return 1;
		}
		strcpy(dot, ".smc");
	}

	FILE *in = fopen(input, "rb");
	if (!in) {
		perror(input);
		return 1;
	}

	uint32_t start = read_uint32(in, ROM_START_OFFSET);
	uint32_t footer = read_uint32(in, FOOTER_START_OFFSET);
	uint32_t rom_size = read_uint32(in, footer + FOOTER_ROM_SIZE_OFFSET);

	if (fseek(in, start, SEEK_SET) != 0) {
		perror("error seeking start of ROM");
		return 1;
	}

	FILE *out = fopen(output, "wb");
	if (!out) {
		perror(output);
		return 1;
	}

	while (rom_size-- > 0) {
		int c = fgetc(in);
		if (c == EOF) {
			perror("error reading input file");
			return 1;
		}

		if (fputc(c, out) == EOF) {
			perror("error writing output file");
			return 1;
		}
	}
}
