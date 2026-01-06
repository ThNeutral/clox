#include "core/vm.h"
#include "utils/memory.h"
#include "debug/disasm.h"

#include <stdio.h>

static Value *get_stack_top(VM *vm)
{
    return vm->stack + vm->stack_count;
}

static void reset_stack(VM *vm)
{
    vm->stack_count = 0;
}

static int get_offset_vm(VM *vm)
{
    return (int)(vm->ip - vm->chunk->code);
}

// TODO: If you want to learn some of these techniques, look up “direct threaded code”, “jump table”, and “computed goto”.
static InterpretResult run(VM *vm)
{
#define READ_BYTE() (*vm->ip++)
#define READ_WORD(size) ({                \
    int word = 0;                         \
    for (int i = 0; i < size; i++)        \
        word = (word << 8) | READ_BYTE(); \
    word;                                 \
})
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
        for (Value *slot = vm->stack; slot < get_stack_top(vm); slot++)
        {
            printf("[ ");
            print_value(*slot);
            printf(" ]");
        }
        printf("\n");
        disassemble_instruction(vm->chunk, get_offset_vm(vm));
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
            Value constant = read_constant(vm->chunk, READ_WORD(3));
            push(vm, constant);
            print_value(constant);
            printf("\n");
            break;
        }
        case OP_NEGATE:
        {
            *(get_stack_top(vm) - 1) *= -1;
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
#undef READ_WORD
}

void grow_stack(VM *vm)
{
    size_t old_capacity = vm->stack_capacity;
    vm->stack_capacity = GROW_CAPACITY_OR_INIT(vm->stack_capacity, STACK_INITIAL_SIZE);
    vm->stack = GROW_ARRAY(Value, vm->stack, old_capacity, vm->stack_capacity);
}

void reset_vm(VM *vm)
{
    vm->chunk = NULL;
    reset_stack(vm);
}

void init_vm(VM *vm)
{
    vm->stack = NULL;
    vm->stack_capacity = 0;
    vm->stack_count = 0;

    grow_stack(vm);
    reset_vm(vm);
}

void free_vm(VM *vm)
{
    FREE_ARRAY(Value, vm->stack, vm->stack_capacity);
    reset_vm(vm);
}

InterpretResult interpret(VM *vm, Chunk *chunk)
{
    vm->chunk = chunk;
    vm->ip = vm->chunk->code;
    return run(vm);
}

void push(VM *vm, Value value)
{
    *get_stack_top(vm) = value;
    vm->stack_count += 1;
}

Value pop(VM *vm)
{
    vm->stack_count -= 1;
    return *get_stack_top(vm);
}