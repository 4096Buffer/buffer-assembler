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


unsigned char lookupOpcode(const char* name);
int readFile(BASM* basm);