#include "../include/assembler.h"


unsigned char lookupOpcode(const char* name) {
	int count = sizeof(OPCODE_TABLE) / sizeof(OPCODE_TABLE[0]);
	for (int i = 0; i < count; i++) {
		if (strcmp(OPCODE_TABLE[i].name, name) == 0) {
			return OPCODE_TABLE[i].opcode;
		}
	}
	return 0;
}

static char* __cstrdup(const char* s) {
	size_t n = strlen(s) + 1;
	char* p = malloc(n);
	if (p) memcpy(p, s, n);
	return p;
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
	basm->size = size;

	return 0;

	//REMEMEBER TO FREE BUFFER
}

int findAddressFromLabel(int* out, Label* labels, int labels_count, char* name) {
	for (int i = 0; i < labels_count; ++i) {
		if (strcmp(labels[i].name, name) == 0) {
			out[0] = (labels[i].address >> 8) & 0xFF;
			out[1] = labels[i].address & 0xFF;
			return 0;
		}
	}
	printf("Error: unknown label '%s'\n", name);
	return 1;
}

int parseData(BASM* basm) {
	
}

int parseInstructions(BASM* basm) {
	char opc_bfr[8] = "";
	char label_bfr[64] = "";
	char arg1_bfr[16] = "";
	char arg2_bfr[16] = "";

	int opcode = -1;
	int type = 0;
	int spaces = 0;
	int b_i = 0;
	int bc_addr = 0;
	int at_line_start = 1;
	int prev_was_space = 1;
	int label_written = 0;

	Label labels[256];
	int label_count = 0;

	for (int i = 0; i < basm->size; i++) {
		unsigned char c = basm->buffer[i];

		if (c == '\n' || c == '\r') {
			spaces = 0;
			at_line_start = 1;
			prev_was_space = 1;
			continue;
		}
		if (c == '\t' || c == '\0') continue;

		if (c == ';') {
			spaces = 0;
			label_bfr[0] = '\0';
			bc_addr += 4;
			at_line_start = 1;
			prev_was_space = 1;
			continue;
		}

		if (c == ':') {
			labels[label_count].name = __cstrdup(label_bfr);
			labels[label_count].address = bc_addr;
			label_bfr[0] = '\0';
			label_count++;
			at_line_start = 1;
			prev_was_space = 1;
			continue;
		}

		if (c == ' ') {
			if (!at_line_start && !prev_was_space) spaces++;
			prev_was_space = 1;
			continue;
		}

		at_line_start = 0;
		prev_was_space = 0;

		size_t len = strlen(label_bfr);
		if (spaces == 0 && len + 1 < sizeof(label_bfr)) {
			label_bfr[len] = c;
			label_bfr[len + 1] = '\0';
		}
	}

	spaces = 0;
	bc_addr = 0;
	at_line_start = 1;
	prev_was_space = 1;
	label_written = 0;

	for (int i = 0; i < basm->size; i++) {
		unsigned char c = basm->buffer[i];

		if (c == ';') {
			if (!label_written) {
				basm->bytecode[b_i++] = (arg1_bfr[0] != '\0') ? (unsigned char)strtol(arg1_bfr, NULL, 0) : 0;
				basm->bytecode[b_i++] = (arg2_bfr[0] != '\0') ? (unsigned char)strtol(arg2_bfr, NULL, 0) : 0;
			}
			label_written = 0;

			spaces = 0;
			type = 0;
			opcode = -1;
			opc_bfr[0] = '\0';
			label_bfr[0] = '\0';
			arg1_bfr[0] = '\0';
			arg2_bfr[0] = '\0';
			bc_addr += 4;
			at_line_start = 1;
			prev_was_space = 1;
			continue;
		}

		if (c == ':') {
			opc_bfr[0] = '\0';
			spaces = 0;
			at_line_start = 1;
			prev_was_space = 1;
			continue;
		}

		if (c == '\n' || c == '\r') {
			spaces = 0;
			at_line_start = 1;
			prev_was_space = 1;
			continue;
		}
		if (c == '\t' || c == '\0') continue;

		if (c == ' ') {
			if (!at_line_start && !prev_was_space) spaces++;
			prev_was_space = 1;
			continue;
		}

		at_line_start = 0;
		prev_was_space = 0;

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
				basm->bytecode[b_i++] = opcode;
			}
			type = c - '0';
			basm->bytecode[b_i++] = type;
		}
		else if (spaces == 2) {
			int first_char_of_label = (label_bfr[0] == '\0') &&
				((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');

			if (first_char_of_label || label_bfr[0] != '\0') {
				size_t len = strlen(label_bfr);
				if (len + 1 < sizeof(label_bfr)) {
					label_bfr[len] = c;
					label_bfr[len + 1] = '\0';
				}
				if (basm->buffer[i + 1] == ';') {
					int parts[2];
					findAddressFromLabel(parts, labels, label_count, label_bfr);
					basm->bytecode[b_i++] = (unsigned char)parts[0];
					basm->bytecode[b_i++] = (unsigned char)parts[1];
					label_bfr[0] = '\0';
					label_written = 1;
				}
				continue;
			}

			size_t len = strlen(arg1_bfr);
			if (len + 1 < sizeof(arg1_bfr)) {
				arg1_bfr[len] = c;
				arg1_bfr[len + 1] = '\0';
			}
		}
		else if (spaces == 3) {
			size_t len = strlen(arg2_bfr);
			if (len + 1 < sizeof(arg2_bfr)) {
				arg2_bfr[len] = c;
				arg2_bfr[len + 1] = '\0';
			}
		}
	}

	basm->bytecode_size = b_i;
	return 0;
}

void saveBin(BASM* basm) {
	FILE* f = fopen("program.bin", "wb");

	if (f == NULL) {
		printf("Error: Couldn't open file for writing.\n");
		return;
	}

	size_t written = fwrite(basm->bytecode, 1, basm->bytecode_size, f);
	if (written != basm->bytecode_size) {
		printf("Error: Wrote %zu of %zu bytes.\n", written, basm->bytecode_size);
	}

	fclose(f);
}