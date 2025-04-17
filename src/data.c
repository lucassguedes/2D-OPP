#include "data.h"

Data* read_instance(char* file_path){

    Data* data = (Data*)malloc(sizeof(Data));

    FILE* file = fopen(file_path, "r");


    unsigned int nitems;

    char line[256];

    fgets(line, sizeof(line), file);
    sscanf(line, "%d", &nitems);

    data->items = (Box*)malloc(sizeof(Box)*nitems);

    fgets(line, sizeof(line), file);
    sscanf(line, "%d %d", &data->width, &data->height);


    while(fgets(line, sizeof(line), file)){
        int i, w, h, d, b, p;
        sscanf(line, "%d %d %d %d %d %d", &i, &w, &h, &d, &b, &p);

        data->items[i-1].width = w;
        data->items[i-1].height = h;
        data->items[i-1].x = 0;
        data->items[i-1].y = 0;
        data->items[i-1].ncopies = b;
        data->items[i-1].packed_items = 0;
        data->items[i-1].rotated = false;
    }

    fclose(file);

    return data;
}


void destroy_data(Data* data){
    free(data->items);
    free(data);
}