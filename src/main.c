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

	if (parseInstructions(&basm)) return 1;

	for (int i = 0; i < basm.size; i++) {
		printf("0x%X\n", (int)basm.bytecode[i]);
	}

	free(basm.buffer);
	free(basm.bytecode);
}