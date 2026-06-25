#include "../include/opcodes.h"
#include "../include/assembler.h"

int main(int argc, char** argv) {
	char path[260];

	if (!argv[1]) {
		printf("Path not specified. Please input the path to the file with instructions: ");
		
		scanf_s("%260s", path, (unsigned)_countof(path));
	}

	BASM basm = { 0 };
	basm.path = path;

	if (readFile(&basm)) return 1;

	char opc_bfr[8] = "";
	int opcode = -1;
	char type_bfr[8] = "";
	int type = -1;
	int arg1 = 0;
	int arg2 = 0;
	int spaces = 0;

	for (int i = 0; i < basm.size; i++) {
		unsigned char c = basm.buffer[i];

		if (c == ';') {
			spaces = 0;
			arg1 = 0;
			arg2 = 0;
			type = -1;
			opcode = -1;
			opc_bfr[0] = '\0';
			type_bfr[0] = '\0';

			printf("###########END###########\n");
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
				printf("OPCODE BFR: %s\n", opc_bfr);
				opcode = lookupOpcode(opc_bfr);
				printf("OPCODE: %d\n", opcode);
			}

			size_t len = strlen(type_bfr);

			if (len + 1 < sizeof(type_bfr)) {
				type_bfr[len] = c;
				type_bfr[len + 1] = '\0';
			}
		}
		else if (spaces == 2) {
			if (type < 0) {
				type = lookupType(type_bfr);

				printf("TYPE: %d\n", type);
			}

			arg1 = c - '0'; // calculate diff from two ascii numbers to get decimal
			printf("ARG1: %d\n", arg1);
		}
		else if (spaces == 3) {
			arg2 = c - '0';

			printf("ARG2: %d\n", arg2);
		}
	}

	free(basm.buffer);
}