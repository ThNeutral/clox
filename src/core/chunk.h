#ifndef clox_chunk_h
#define clox_chunk_h

#include "utils/common.h"
#include "utils/rle.h"
#include "core/value.h"

typedef enum
{
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN,
} OpCode;

typedef struct
{
    int count;
    int capacity;
    uint8_t *code;

    RLEArray lines;

    ValueArray constants;
} Chunk;

void init_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte, int line);
void free_chunk(Chunk *chunk);

int get_line(Chunk *chunk, int index);

void write_constant(Chunk *chunk, Value value, int line);
Value read_constant(Chunk *chunk, int index);
int get_constant_index(Chunk *chunk, int offset, int size);

#endif