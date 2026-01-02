#include "core/chunk.h"
#include "debug/disasm.h"

int main(int argc, const char *argv[])
{
    Chunk chunk;

    init_chunk(&chunk);

    int constant = add_constant(&chunk, 1.2);
    write_chunk(&chunk, OP_CONSTANT, 123);
    write_chunk(&chunk, constant, 123);

    write_chunk(&chunk, OP_CONSTANT, 123);
    write_chunk(&chunk, constant, 123);

    write_chunk(&chunk, OP_CONSTANT, 124);
    write_chunk(&chunk, constant, 124);

    write_chunk(&chunk, OP_RETURN, 124);

    disassemble_chunk(&chunk, "Test Chunk");

    free_chunk(&chunk);

    return 0;
}