#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "opcodes.h"

typedef struct {
	char* name;
	unsigned int address;
} Label;

typedef struct {
	char* buffer;
	char* path;
	long size;
	unsigned char* bytecode;
	unsigned char data[1024];
	Label data_labels[256];
	int data_label_count;
	size_t bytecode_size;
} BASM;


unsigned char lookupOpcode(const char* name);
int readFile(BASM* basm);
int parseInstructions(BASM* basm);
int parseData(BASM* basm);
void findAddressFromLabel(int* out, Label* labels, int labels_count, char* name);
void saveBin(BASM* basm);