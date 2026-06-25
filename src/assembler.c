#include "../include/assembler.h"

unsigned char lookupType(const char* name) {
	int count = sizeof(TYPE_TABLE) / sizeof(TYPE_TABLE[0]);
	for (int i = 0; i < count; i++) {
		if (strcmp(TYPE_TABLE[i].name, name) == 0) {
			return TYPE_TABLE[i].value;
		}
	}
	return -1;
}

unsigned char lookupOpcode(const char* name) {
	int count = sizeof(OPCODE_TABLE) / sizeof(OPCODE_TABLE[0]);
	for (int i = 0; i < count; i++) {
		if (strcmp(OPCODE_TABLE[i].name, name) == 0) {
			return OPCODE_TABLE[i].opcode;
		}
	}
	return -1;
}

int readFile(BASM* basm) {
	FILE* file = fopen(basm->path, "rb");

	if (file == NULL) {
		printf("Error: Couldn't open the file.\n");
		return 1;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);
                    
	if (size < 0) {
		printf("Error: ftell failed.\n");
		fclose(file);
		return 1;
	}
	
	char* buffer = malloc(size + 1);
	if (buffer == NULL) {
		printf("Error: Couldn't allocate %ld bytes on heap!\n", size + 1);
		fclose(file);
		return 1;
	}

	fread(buffer, 1, size, file);
	buffer[size] = '\0';

	fclose(file);

	basm->buffer = buffer;
	basm->size = size + 1;

	return 0;

	//REMEMEBER TO FREE BUFFER
}