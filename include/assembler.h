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
} BASM;


unsigned char lookupType(const char* name);
unsigned char lookupOpcode(const char* name);
int readFile(BASM* basm);