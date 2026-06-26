#include "../include/assembler.h"


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

void findAddressFromLabel(int* out, Label* labels, int labels_count, char* name) {
	unsigned int address = 0;

	for(int i = 0; i < labels_count; ++i) {
		if (strcmp(labels[i].name, name) == 0) {
			address = labels[i].address;
		}
	}

	out[0] = (address >> 8) & 0xFF;
	out[1] = address & 0xFF;
}

int parseInstructions(BASM* basm) {
	char opc_bfr[8] = "";
	char label_bfr[64] = "";

	int opcode = -1;
	int type = 0;
	int arg1 = 0;
	int arg2 = 0;
	int spaces = 0;
	int b_i = 0;
	int bc_addr = 0;

	Label labels[256]; //change to dynamic alloc in future if needed
	int label_count = 0;

	//ADD 2 LOOPS FIRST TO GET LABELS AND SECOND TO PARSE INSTRUCTIONS BECAUSE NOW YOU CANT CALL A FUNCTION BEFORE DECLARATION IN CODE

	for (int i = 0; i < basm->size; i++) {
		unsigned char c = basm->buffer[i];

		if (c == '\n' || c == '\r' || c == '\t' || c == '\0') continue;

		if (c == ';') {
			spaces = 0;
			label_bfr[0] = '\0';
			bc_addr += 4;

			continue;
		}
		
		if (c == ':') {
			labels[label_count].name = _strdup(label_bfr);
			labels[label_count].address = bc_addr;

			printf("NEW LABEL: %s\n", label_bfr);

			label_bfr[0] = '\0';
			label_count++;

			
			continue;
		}

		if (c == ' ') {
			spaces++;
			continue;
		}

		size_t len = strlen(label_bfr);

		if (spaces == 0 && len + 1 < sizeof(label_bfr)) {
			label_bfr[len] = c;
			label_bfr[len + 1] = '\0';
		}
	}

	spaces = 0;
	bc_addr = 0;

	for (int i = 0; i < basm->size; i++) {
		unsigned char c = basm->buffer[i];

		if (c == ';') {
			spaces = 0;
			arg1 = 0;
			arg2 = 0;
			type = 0;
			opcode = -1;
			opc_bfr[0] = '\0';
			label_bfr[0] = '\0';

			bc_addr += 4;

			continue;
		}

		if (c == ':') {
			opc_bfr[0] = '\0';
			spaces = 0;
			continue;
		}

		if (c == '\n' || c == '\r' || c == '\t' || c == '\0') continue;

		if (c == ' ') {
			spaces++;
			continue;
		}

		if (spaces == 0) {
			size_t len = strlen(opc_bfr);

			if (len + 1 < sizeof(opc_bfr)) {
				opc_bfr[len] = c;
				opc_bfr[len + 1] = '\0';
			}
		}
		else if (spaces == 1) {
			if (opcode < 0) {
				opcode = lookupOpcode(opc_bfr);

				basm->bytecode[b_i] = opcode;
				b_i++;
				printf("OPCODE: %s\n", opc_bfr);
			}

			type = c - '0';

			basm->bytecode[b_i] = type;
			b_i++;

			printf("TYPE: %d\n", type);
		}
		else if (spaces == 2) {
			if (c == '_' || strlen(label_bfr) > 0) {
				printf("FUNC DETECT\n");
				if (basm->buffer[i + 1] == ';') {
					int parts[2];

					findAddressFromLabel(parts, labels, label_count, label_bfr);

					arg1 = parts[0];
					arg2 = parts[1];
				}
				else {
					size_t len = strlen(label_bfr);

					if (len + 1 < sizeof(label_bfr)) {
						label_bfr[len] = c;
						label_bfr[len + 1] = '\0';
					}
				}
				continue;
			}
			arg1 = c - '0'; // calculate diff from two ascii numbers to get decimal
			basm->bytecode[b_i] = arg1;
			b_i++;

			printf("ARG1: %d\n", arg1);
		}
		else if (spaces == 3) {
			arg2 = c - '0';
			basm->bytecode[b_i] = arg2;
			b_i++;

			printf("ARG2: %d\n", arg2);
		}
	}

	return 0;
}