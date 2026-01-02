#ifndef DISASM_H
#define DISASM_H

#include "core/chunk.h"
#include "core/value.h"

void disassemble_chunk(Chunk *chunk, const char *name);
int disassemble_instruction(Chunk *chunk, int offset);

#endif