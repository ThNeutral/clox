#include "debug/disasm.h"

#include <stdio.h>

static int simple_instruction(const char *name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

static int constant_instruction(const char *name, Chunk *chunk, int offset, int size)
{
    int index = get_constant_index(chunk, offset, size);
    Value value = read_constant(chunk, index);

    printf("%-16s %4d '", name, index);
    print_value(value);
    printf("'\n");

    return offset + 1 + size;
}

void disassemble_chunk(Chunk *chunk, const char *name)
{
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;)
    {
        offset = disassemble_instruction(chunk, offset);
    }
}

int disassemble_instruction(Chunk *chunk, int offset)
{
    printf("%04d ", offset);
    if (offset > 0 && get_line(chunk, offset) == get_line(chunk, offset - 1))
    {
        printf("   | ");
    }
    else
    {
        printf("%4d ", get_line(chunk, offset));
    }

    uint8_t instruction = chunk->code[offset];

    switch (instruction)
    {
    case OP_RETURN:
        return simple_instruction("OP_RETURN", offset);

    case OP_CONSTANT:
        return constant_instruction("OP_CONSTANT", chunk, offset, 1);

    case OP_CONSTANT_LONG:
        return constant_instruction("OP_CONSTANT_LONG", chunk, offset, 3);

    case OP_NEGATE:
        return simple_instruction("OP_NEGATE", offset);

    case OP_ADD:
        return simple_instruction("OP_ADD", offset);

    case OP_DIVIDE:
        return simple_instruction("OP_DIVIDE", offset);

    case OP_MULTIPLY:
        return simple_instruction("OP_MULTIPLY", offset);

    case OP_SUBTRACT:
        return simple_instruction("OP_SUBTRACT", offset);

    default:
        printf("Unknown opcode: %d\n", instruction);
        return offset + 1;
    }
}