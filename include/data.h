#ifndef __DATA_H__
#define __DATA_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int width;
    int height;
    int ncopies; // maximum number of copies of item i
    bool rotated;
}Box;


typedef struct {
    int width;
    int height;
    int nunique_items;
    int nitems; // Number of items, considering repetitions

    Box* items;
}Data;


Data* read_instance(char* file_path);
void  destroy_data(Data* data);

#endif