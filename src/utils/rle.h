#ifndef RLE_H
#define RLE_H

#include <limits.h>
#define RLE_INVALID_VALUE INT_MIN

typedef struct
{
    int key;
    int size;
} RLENode;

typedef struct
{
    int capacity;
    int count;
    RLENode *values;
} RLEArray;

void init_rle_array(RLEArray *array);
void write_rle_array(RLEArray *array, int value);
int read_rle_array(RLEArray *array, int index);
void free_rle_array(RLEArray *array);

#endif