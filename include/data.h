#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int width;
    int height;
    bool rotated;
    int ncopies; // maximum number of copies of item i
    int packed_items;
}Box;


typedef struct {
    int width;
    int height;

    Box* items;
}Data;


Data* read_instance(char* file_path);
void  destroy_data(Data* data);