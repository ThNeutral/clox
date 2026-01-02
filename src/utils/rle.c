#include "utils/rle.h"
#include "utils/memory.h"

#include <stdio.h>

void rle_node_init(RLENode *node, int key)
{
    node->key = key;
    node->size = 1;
}

void init_rle_array(RLEArray *array)
{
    array->capacity = 0;
    array->count = 0;
    array->values = NULL;
}

void write_rle_array(RLEArray *array, int value)
{
    if (array->count >= array->capacity)
    {
        int old_capacity = array->capacity;
        array->capacity = GROW_CAPACITY(old_capacity);
        array->values = GROW_ARRAY(RLENode, array->values, old_capacity, array->capacity);
    }

    if (array->count > 0)
    {
        RLENode *last_node = &array->values[array->count - 1];
        if (last_node->key == value)
        {
            last_node->size += 1;
            return;
        }
    }

    RLENode new_node;
    rle_node_init(&new_node, value);

    array->values[array->count] = new_node;
    array->count += 1;
}

int read_rle_array(RLEArray *array, int index)
{
    if (index < 0 || array->count == 0)
    {
        return RLE_INVALID_VALUE;
    }

    int cursor = 0;
    for (int i = 0; i < array->count; i++)
    {
        int next_boundary = cursor + array->values[i].size;

        if (index >= cursor && index < next_boundary)
        {
            return array->values[i].key;
        }
        cursor = next_boundary;
    }

    return RLE_INVALID_VALUE;
}

void free_rle_array(RLEArray *array)
{
    FREE_ARRAY(RLENode, array->values, array->capacity);
    init_rle_array(array);
}
