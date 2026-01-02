#include "core/vm.h"
#include "utils/common.h"
#include "debug/disasm.h"

#include <stdio.h>

static void reset_stack(VM *vm)
{
    vm->stack_top = vm->stack;
}

// TODO: If you want to learn some of these techniques, look up “direct threaded code”, “jump table”, and “computed goto”.
static InterpretResult run(VM *vm)
{
#define READ_BYTE() (*vm->ip++)
#define BINARY_OP(op)       \
    do                      \
    {                       \
        double b = pop(vm); \
        double a = pop(vm); \
        push(vm, a op b);   \
    } while (false)

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value *slot = vm->stack; slot < vm->stack_top; slot++)
        {
            printf("[ ");
            print_value(*slot);
            printf(" ]");
        }
        printf("\n");
        disassemble_instruction(vm->chunk, (int)(vm->ip - vm->chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {
        case OP_CONSTANT:
        {
            Value constant = read_constant(vm->chunk, READ_BYTE());
            push(vm, constant);
            print_value(constant);
            printf("\n");
            break;
        }
        case OP_CONSTANT_LONG:
        {
            // TODO: NOT WORKING
            int index = get_constant_index(vm->chunk, (int)(vm->ip - vm->chunk->code), 3);
            for (int i = 0; i < 3; i++)
                READ_BYTE();

            Value constant = read_constant(vm->chunk, index);
            push(vm, constant);
            print_value(constant);
            printf("\n");
            break;
        }
        case OP_NEGATE:
        {
            push(vm, -pop(vm));
            break;
        }
        case OP_ADD:
        {

            BINARY_OP(+);
            break;
        }
        case OP_SUBTRACT:
        {
            BINARY_OP(-);
            break;
        }
        case OP_MULTIPLY:
        {
            BINARY_OP(*);
            break;
        }
        case OP_DIVIDE:
        {
            BINARY_OP(/);
            break;
        }
        case OP_RETURN:
        {
            print_value(pop(vm));
            printf("\n");
            return INTERPRET_OK;
        }
        }
    }

#undef BINARY_OP
#undef READ_BYTE
}

void init_vm(VM *vm)
{
    vm->chunk = NULL;
    reset_stack(vm);
}

void free_vm(VM *vm)
{
    init_vm(vm);
}

InterpretResult interpret(VM *vm, Chunk *chunk)
{
    vm->chunk = chunk;
    vm->ip = vm->chunk->code;
    return run(vm);
}

void push(VM *vm, Value value)
{
    *vm->stack_top = value;
    vm->stack_top += 1;
}

Value pop(VM *vm)
{
    vm->stack_top -= 1;
    return *vm->stack_top;
}