#include "best_fit.h"

int compare_boxes(const void* a, const void* b){
    const Box* b1 = (const Box*)a;
    const Box* b2 = (const Box*)b;

    return b2->width - b1->width;
}

GapInfo find_lowest_gap(int niches[], int n_niches){
    int lowest_gap_height = INT_MAX;
    GapInfo info;

    info.width = 0;
    for(int i = 0; i < n_niches; i++){
        if(niches[i] < lowest_gap_height){
            lowest_gap_height = niches[i];
            info.index = i;
            info.width = 1;
        }else if(niches[i] == lowest_gap_height){
            info.width++;
        }
    }

    return info;
}

InsertionInfo find_best_fit_idx(GapInfo gap, Box items[], int nitems){
    InsertionInfo info;
    
    info.index = GAP_NOT_FOUND;
    int best_diff = INT_MAX;
    int diff;
    for(int i = 0; i < nitems; i++){

        if(!items[i].ncopies) continue;

        if(items[i].width > gap.width){
            diff = abs(items[i].height - gap.width);
            if(items[i].height < gap.width && diff < best_diff){
                best_diff = diff;
                info.index = i;
                info.rotated = !items[i].rotated;
            }
        }else{
            diff = abs(items[i].width - gap.width);
            if(diff < best_diff){
                best_diff = diff;
                info.index = i;
                info.rotated = items[i].rotated;
            }
        }
    }    

    return info;
}

int get_x_value(GapInfo gap, PlacementPolicy policy, int item_width, int niches[], int n_niches){
    switch(policy){
        case LEFTMOST_POLICY:
            return gap.index; //Place the item on the leftmost side
        case TALLEST_NEIGHBOR_POLICY:
            //If the right side is the border of the container, we choose to place the item next to it
            if(gap.index + gap.width > n_niches){
                return gap.index + gap.width - item_width - 1;
            }

            //If the left side is the border of the container, we choose to place the item next to it
            if(gap.index - 1 < 0){
                return gap.index;
            }

            //If the height of the right side is greater than the left side, we place the item on the right
            if(niches[gap.index + gap.width] > niches[gap.index - 1]){
                return gap.index + gap.width - item_width - 1;
            }

            // Place in the left side
            return gap.index;
        case SHORTEST_NEIGHBOR_POLICY:
            //If the right side is the border of the container, we choose to place the item on the left
            if(gap.index + gap.width > n_niches){
                return gap.index;
            }

            //If the left side is the border of the container, we choose to place the item on the right
            if(gap.index - 1 < 0){
                return gap.index + gap.width - item_width - 1;
            }

            //If the height of the right side is greater than the left side, we place the item on the right
            if(niches[gap.index + gap.width] > niches[gap.index - 1]){
                return gap.index;
            }

            // Place in the left side
            return gap.index + gap.width - item_width - 1;
        default:
            return -1;
    }
}

void update_niches(int niches[], GapInfo gap, InsertionInfo info, int item_width, int item_height){
    for(int i = gap.index; i <= gap.index + item_width - 1; i++){
        niches[i] += item_height;
    }
}   

void ignore_gap(GapInfo gap, int niches[], int n_niches){
    int lowest_neighbor_height = INT_MAX;

    if(gap.index - 1 < 0){
        lowest_neighbor_height = niches[gap.index + gap.width];
    }else if(gap.index + gap.width > n_niches){
        lowest_neighbor_height = niches[gap.index - 1];
    }else{
        if(niches[gap.index + gap.width] < niches[gap.index - 1]){
            lowest_neighbor_height = niches[gap.index + gap.width];
        }else{
            lowest_neighbor_height = niches[gap.index - 1];
        }
    }

    for(int i = gap.index; i < gap.index + gap.width; i++){
        niches[i] += lowest_neighbor_height;
    }
}

BestFitResult* best_fit(Data* data){

    BestFitResult* result = (BestFitResult*)malloc(sizeof(BestFitResult));

    result->items_status = (BoxStatus*)malloc(sizeof(BoxStatus)*data->nunique_items);
    int packed_items = 0;

    const int n_niches = data->width;


    int niches[n_niches];

    /*All niches have 0 height values in the beginning*/
    for(int i = 0; i < data->width; i++) niches[i] = 0;

    Box items[data->nitems];

    /*Copying the items to a local variable*/
    int item_counter = 0; 
    for(int i = 0; i < data->nitems; i++){

        items[item_counter] = data->items[i];
        if(items[item_counter].height > items[item_counter].width){
            /*Rotate the item if its height is greater than its width*/
            int aux = items[item_counter].height;
            items[item_counter].height = items[item_counter].width;
            items[item_counter].width = aux;
            items[item_counter].rotated = true;
        }
        item_counter++;
    }

    qsort(items, data->nitems, sizeof(Box), compare_boxes);

    printf("All items:\n");
    for(int i = 0; i < data->nitems; i++){
        printf("Item %d - width = %d, height = %d\n", i, items[i].width, items[i].height);
    }

    GapInfo gap_info;
    InsertionInfo insertion_info;
    int x, y, item_width, item_height;
    PlacementPolicy policy = LEFTMOST_POLICY;


    while(packed_items < data->nunique_items){
        gap_info = find_lowest_gap(niches, n_niches);

        y = niches[gap_info.index];
        
        insertion_info = find_best_fit_idx(gap_info, items, data->nitems);

        if(insertion_info.index == ITEM_NOT_FOUND){
            printf("Item not found...\n");
            ignore_gap(gap_info, niches, n_niches);
            continue;
        }

        if(insertion_info.rotated != items[insertion_info.index].rotated){
            item_width = items[insertion_info.index].height;
            item_height = items[insertion_info.index].width;
        }else{
            item_width = items[insertion_info.index].width;
            item_height = items[insertion_info.index].height;
        }


        x = get_x_value(gap_info, policy, item_width, niches, n_niches);
        
        
        items[insertion_info.index].ncopies--;
        packed_items++;
        printf("Insertion_info.index: %d\n", insertion_info.index);
        printf("Item width = %d, Item height: %d\n", item_width, item_height);

        update_niches(niches, gap_info, insertion_info, item_width, item_height);

        printf("Item to insert:\n");
        printf("Item %d - width = %d, height = %d ", insertion_info.index, item_width, item_height);
        printf("at position x = %d and y = %d\n", x, y);

        printf("Niches: ");
        for(int i = 0; i < n_niches; i++){
            printf("%3d ", niches[i]);
        }
        printf("\n");
    }

    return result;
}