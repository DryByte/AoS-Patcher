#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	FILE* fp_r = fopen(argv[1], "r");
	if (!fp_r) {
		printf("ERROR: Can't open '%s'\n", argv[1]);
		return 1;
	}

	fseek(fp_r, 0, SEEK_END);
	int size = ftell(fp_r);
	rewind(fp_r);

	char* file_buf = malloc(size);

	fread(file_buf, 1, size, fp_r);
	fclose(fp_r);

	// CodeCave at 0x401020
	char buf[] = {
		0x68, 0xD4, 0x65, 0x44, 0x0, //push 0x4465d4 (LIBAOS.DLL)
		0xff, 0x15, 0x48, 0x40, 0x44, 0x0, // call LoadLibraryW
		0x6a, 0x58, // push 0x58
		0x68, 0x98, 0xdd, 0x44, 0x0, // push 0x44dd98
		0xe9, 0x06, 0xa3, 0x0, 0x0 // jmp 0x40b33d (back to the entry)
	};
	memcpy(file_buf+0x420, buf, 23);

	// jumps at entry 0x40b336
	char jmp_buf[] = {
		0xe9, 0xe5, 0x5c, 0xff, 0xff, // jmp 0x401020
		0x90, 0x90
	};
	memcpy(file_buf+0x9b36, jmp_buf, 7);

	// String
	char libaos_str[] = {
		'L',0,'I',0,'B',0,'A',0,'O',0,'S',0,'.',0,'D',0,'L',0,'L'
	};
	memcpy(file_buf+0x441d4, libaos_str, 19);

	FILE* fp_w = fopen(argv[1], "w");
	fwrite(file_buf, 1, size, fp_w);

	free(file_buf);
	fclose(fp_w);

	printf("Patched %s succesfuly! Don't forget to copy libaos.dll to the same folder.\n", argv[1]);
	return 0;
}