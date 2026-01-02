#ifndef clox_memory_h
#define clox_memory_h

#include "utils/common.h"
#include <stdlib.h>

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, old_count, new_count)     \
    (type *)reallocate(pointer, sizeof(type) * (old_count), \
                       sizeof(type) * (new_count))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void *reallocate(void *pointer, size_t oldSize, size_t newSize);

#endif