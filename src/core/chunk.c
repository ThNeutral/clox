#include "core/chunk.h"
#include "utils/memory.h"

#include <stdio.h>

void init_chunk(Chunk *chunk)
{
    chunk->capacity = 0;
    chunk->count = 0;
    chunk->code = NULL;
    init_rle_array(&chunk->lines);
    init_value_array(&chunk->constants);
}

void write_chunk(Chunk *chunk, uint8_t byte, int line)
{
    if (chunk->capacity < chunk->count + 1)
    {
        int old_capacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old_capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
    }

    write_rle_array(&chunk->lines, line);

    chunk->code[chunk->count] = byte;
    chunk->count += 1;
}

void write_constant(Chunk *chunk, Value value, int line)
{
    int index = add_constant(chunk, value);

    if (index <= 255)
    {
        write_chunk(chunk, OP_CONSTANT, line);
        write_chunk(chunk, index, line);
        return;
    }

    write_chunk(chunk, OP_CONSTANT_LONG, line);
    write_chunk(chunk, (index >> 16) & 0xFF, line);
    write_chunk(chunk, (index >> 8) & 0xFF, line);
    write_chunk(chunk, (index & 0xFF), line);
}

int get_line(Chunk *chunk, int index)
{
    int line = read_rle_array(&chunk->lines, index);
    if (line == RLE_INVALID_VALUE)
    {
        printf("get_line: Bad read");
        exit(1);
    }
    return line;
}

void free_chunk(Chunk *chunk)
{
    free_value_array(&chunk->constants);
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    free_rle_array(&chunk->lines);
    init_chunk(chunk);
}

int add_constant(Chunk *chunk, Value value)
{
    write_value_array(&chunk->constants, value);
    return chunk->constants.count - 1;
}