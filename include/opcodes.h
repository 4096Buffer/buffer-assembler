#pragma once

#define STR_(x) #x     
#define STR(x) STR_(x)

#define END  0x0
#define MOV  0x1
#define ADD  0x2
#define SUB  0x3
#define MUL  0x4
#define XOR  0x5
#define CMP  0x6
#define JE   0x7
#define JMP  0x8
#define JG   0x9
#define JL   0xA
#define DIV  0xB
#define JNE  0xC
#define JLE  0xD
#define JGE  0xE
#define PUSH 0xF
#define POP  0x10
#define CALL 0x11
#define RET  0x12
#define LOAD  0x13
#define STORE 0x14
#define PRINT 0x15
#define AND  0x16
#define OR   0x17
#define NOT  0x18
#define SHL  0x19
#define SHR  0x1A

#define TYPE_IMM 0x0
#define TYPE_REG 0x1
#define TYPE_MEM 0x2

typedef struct { const char* name; unsigned char opcode; } OpcodeEntry;
typedef struct { const char* name; unsigned char value; } TypeEntry;

static const OpcodeEntry OPCODE_TABLE[] = {
    { "END",   0x00 }, { "MOV",   0x01 }, { "ADD",   0x02 },
    { "SUB",   0x03 }, { "MUL",   0x04 }, { "XOR",   0x05 },
    { "CMP",   0x06 }, { "JE",    0x07 }, { "JMP",   0x08 },
    { "JG",    0x09 }, { "JL",    0x0A }, { "DIV",   0x0B },
    { "JNE",   0x0C }, { "JLE",   0x0D }, { "JGE",   0x0E },
    { "PUSH",  0x0F }, { "POP",   0x10 }, { "CALL",  0x11 },
    { "RET",   0x12 }, { "LOAD",  0x13 }, { "STORE", 0x14 },
    { "PRINT", 0x15 }, { "AND",   0x16 }, { "OR",    0x17 },
    { "NOT",   0x18 }, { "SHL",   0x19 }, { "SHR",   0x1A },
};

static const TypeEntry TYPE_TABLE[] = {
	{ "IMM", TYPE_IMM },  // 0x0
	{ "REG", TYPE_REG },  // 0x1
	{ "MEM", TYPE_MEM },  // 0x2
};