#include "../include/opcodes.h"
#include "../include/assembler.h"

int main(int argc, char** argv) {

	// MOVE IT TO OTHER FUNCTION ON ASSEMBLER.C
	char path[260];

	if (!argv[1]) {
		printf("Path not specified. Please input the path to the file with instructions: ");
		
		scanf_s("%260s", path, (unsigned)_countof(path));
	}

	BASM basm = { 0 };
	basm.path = path;

	if (readFile(&basm)) return 1;

	basm.bytecode = malloc(basm.size);

	char opc_bfr[8] = "";
	int opcode = -1;
	int type = 0;
	int arg1 = 0;
	int arg2 = 0;
	int spaces = 0;
	int b_i = 0;

	for (int i = 0; i < basm.size; i++) {
		unsigned char c = basm.buffer[i];

		if (c == ';') {
			spaces = 0;
			arg1 = 0;
			arg2 = 0;
			type = 0;
			opcode = -1;
			opc_bfr[0] = '\0';

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

				basm.bytecode[b_i] = opcode;
				b_i++;
				printf("OPCODE: %d\n", opcode);
			}

			type = c - '0';

			basm.bytecode[b_i] = type;
			b_i++;

			printf("TYPE: %d\n", type);
		}
		else if (spaces == 2) {
			arg1 = c - '0'; // calculate diff from two ascii numbers to get decimal
			basm.bytecode[b_i] = arg1;
			b_i++;

			printf("ARG1: %d\n", arg1);
		}
		else if (spaces == 3) {
			arg2 = c - '0';
			basm.bytecode[b_i] = arg2;
			b_i++;

			printf("ARG2: %d\n", arg2);
		}
	}

	free(basm.buffer);
	free(basm.bytecode);
}