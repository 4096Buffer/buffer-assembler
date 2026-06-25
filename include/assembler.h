#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "opcodes.h"

typedef struct {
	char* buffer;
	char* path;
	long size;
	unsigned char* bytecode;
} BASM;

typedef struct {
	char* name;
	unsigned int address;
} Label;


unsigned char lookupOpcode(const char* name);
int readFile(BASM* basm);
int parseInstructions(BASM* basm);
void findAddressFromLabel(int* out, Label* labels, int labels_count, char* name);