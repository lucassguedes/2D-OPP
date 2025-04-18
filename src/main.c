#include <stdio.h>
#include "data.h"
#include "best_fit.h"
#include "drawer.h"

int main(int argc, char** argv){
    Data* data = read_instance(argv[1]);

    BestFitResult* bf_result = best_fit(data);


    BoxStatus* items_status = bf_result->items_status;

    printf("Boxes status:\n");

    for(int i = 0; i < data->nunique_items; i++){
        printf("Box status - index = %d, x = %d, y = %d, rotated = %d\n", items_status[i].index, items_status[i].x, items_status[i].y, items_status[i].rotated);
    }

    draw(data, items_status);

    destroy_data(data);

    return 0;
}