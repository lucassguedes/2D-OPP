#include "data.h"

Data* read_instance(char* file_path){

    Data* data = (Data*)malloc(sizeof(Data));

    FILE* file = fopen(file_path, "r");

    char line[256];

    fgets(line, sizeof(line), file);
    sscanf(line, "%d", &data->nunique_items);

    data->items = (Box*)malloc(sizeof(Box)*data->nunique_items);

    fgets(line, sizeof(line), file);
    sscanf(line, "%d %d", &data->width, &data->height);


    while(fgets(line, sizeof(line), file)){
        int i, w, h, d, b, p;
        sscanf(line, "%d %d %d %d %d %d", &i, &w, &h, &d, &b, &p);

        data->items[i-1].unique_id = i;
        data->items[i-1].width = w;
        data->items[i-1].height = h;
        data->items[i-1].ncopies = b;
        data->items[i-1].rotated = false;

        printf("i = %d, w = %d, h = %d, d = %d, b = %d, p = %d\n", i, w, h, d, b, p);

        data->nitems += b;
    }

    fclose(file);

    return data;
}


void destroy_data(Data* data){
    free(data->items);
    free(data);
}