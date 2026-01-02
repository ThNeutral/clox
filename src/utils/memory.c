#include "utils/memory.h"

#include <stdio.h>

void *reallocate(void *pointer, size_t oldSize, size_t newSize)
{
    if (newSize == 0)
    {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, newSize);
    if (result == NULL)
    {
        printf("reallocate: Allocation of %zu bytes failed", newSize);
        exit(1);
    }
    return result;
}