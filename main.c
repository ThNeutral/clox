#include "core/chunk.h"
#include "core/vm.h"
#include "debug/disasm.h"

int main(int argc, const char *argv[])
{
    VM vm;
    init_vm(&vm);

    Chunk chunk;
    init_chunk(&chunk);
    write_constant(&chunk, 1.2, 123);
    write_chunk(&chunk, OP_NEGATE, 123);
    write_constant(&chunk, 2.4, 123);
    write_chunk(&chunk, OP_ADD, 123);
    write_chunk(&chunk, OP_RETURN, 123);

    // disassemble_chunk(&chunk, "Test");
    interpret(&vm, &chunk);

    free_chunk(&chunk);
    free_vm(&vm);

    return 0;
}