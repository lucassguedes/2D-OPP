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
    int i = 0;
    while(i < n_niches){
        if(niches[i] < lowest_gap_height){
            lowest_gap_height = niches[i];
            info.index = i;
            info.width = 0;
            printf("Lowest gap: %d, index: %d\n", lowest_gap_height, i);

            while(niches[i] == lowest_gap_height){
                i++;
                info.width++;
            }
            continue;
        }
        i++;
    }

    return info;
}

InsertionInfo find_best_fit_idx(GapInfo gap, Box items[], int nitems){
    InsertionInfo info;
    
    info.index = GAP_NOT_FOUND;
    int best_diff = INT_MAX;
    int diff;

    printf("Gap index: %d, Gap width: %d\n", gap.index, gap.width);
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
    printf("Ignoring the gap at pos %d with width %d\n", gap.index, gap.width);
    if(gap.index - 1 < 0){
        lowest_neighbor_height = niches[gap.index + gap.width];
    }else if(gap.index + gap.width >= n_niches){
        lowest_neighbor_height = niches[gap.index - 1];
    }else{
        if(niches[gap.index + gap.width] < niches[gap.index - 1]){
            lowest_neighbor_height = niches[gap.index + gap.width];
        }else{
            lowest_neighbor_height = niches[gap.index - 1];
        }
    }

    for(int i = gap.index; i < gap.index + gap.width; i++){
        printf("Incrementing %d to the height....\n", lowest_neighbor_height);
        niches[i] = lowest_neighbor_height;
    }
}

int get_highest_niche(int niches[], int n_niches){
    int highest_niche = INT_MIN;

    for(int i = 0; i < n_niches; i++){
        if(niches[i] > highest_niche){
            highest_niche = niches[i];
        }
    }

    return highest_niche;
}

void reduce_height(Data* data, GapInfo gap_info, BestFitResult* policy_result[], PlacementPolicy curr_policy, int niches[], int n_niches){
    int x, y;
    int current_highest_niche;

    bool skip_item_selection = false;


    int item_height, item_width;
    int highest_tower_idx;
    int max_height;
    int tower_height, selected_tower_width, selected_tower_height;
    BoxStatus* prev_status = NULL;
    /*Solution improvement*/

    int prev_highest_niche = get_highest_niche(niches, n_niches);

    int niches_backup[n_niches];

    for(int i = 0; i < n_niches; i++){
        niches_backup[i] = niches[i];
    }
    int tower_x;
    while(true){
        max_height = INT_MIN;
    
        if(!skip_item_selection){
            //Find the highest tower
            printf("####### Selecting new tower\n");
            for(int i = 0; i < data->nitems; i++){
                BoxStatus* status = policy_result[curr_policy]->items_status[i];
                Box box = data->items[policy_result[curr_policy]->items_status[i]->index - 1];
    
                if(status->rotated){
                    item_height = box.width;
                    item_width = box.height;
                }else{
                    item_height = box.height;
                    item_width = box.width;
                }
    
                tower_height = status->y + item_height;
    
                if(tower_height > max_height){
                    highest_tower_idx = i;
                    max_height = tower_height;
                    selected_tower_width = item_width;
                    selected_tower_height = item_height;
                }
            }

            if(selected_tower_height <= selected_tower_width){
                break;
            }  

            /*Remove the tower (update the niches)*/
            prev_status = policy_result[curr_policy]->items_status[highest_tower_idx];
            tower_x = policy_result[curr_policy]->items_status[highest_tower_idx]->x;
            for(int i = tower_x; i < tower_x + selected_tower_width; i++){
                niches[i] -= selected_tower_height;
            }

            /*Rotate the item*/
            printf("\033[0;35mROTACIOANDO.....\033[0m\n");
            policy_result[curr_policy]->items_status[highest_tower_idx]->rotated = !policy_result[curr_policy]->items_status[highest_tower_idx]->rotated;
            
            /*Swap width and height*/
            int aux = selected_tower_height;
            selected_tower_height = selected_tower_width;
            selected_tower_width = aux;
        }

        printf("Selected tower - Width = %d, Height: %d\n", selected_tower_width, selected_tower_height);  


        /*Find the lowest gap*/
        gap_info = find_lowest_gap(niches, n_niches);

        printf("GAP - idx = %d, width = %d\n", gap_info.index, gap_info.width);


        if(selected_tower_width > gap_info.width){
            printf("Improvement routine - Could not place the item in the gap(index = %d, width = %d)... ignoring it...\n", gap_info.index, gap_info.width);
            ignore_gap(gap_info, niches, n_niches);
            printf("Niches: ");
            for(int i = 0; i < n_niches; i++){
                printf("%3d ", niches[i]);
            }
            printf("\n");
            getchar();
            skip_item_selection = true;
            continue;
        }

        skip_item_selection = false;

        x = get_x_value(gap_info, curr_policy, selected_tower_width, niches, n_niches);
        y = niches[gap_info.index];

        policy_result[curr_policy]->items_status[highest_tower_idx]->x = x;
        policy_result[curr_policy]->items_status[highest_tower_idx]->y = y;

        InsertionInfo insertion_info = {highest_tower_idx, policy_result[curr_policy]->items_status[highest_tower_idx]->rotated};


        printf("Inserindo item na posição x = %d, y = %d\n", x, y);


        update_niches(niches, gap_info, insertion_info, selected_tower_width, selected_tower_height);

        printf("\033[0;31mNiches: ");
        for(int u = 0; u < n_niches; u++){
            printf("%d ", niches[u]);
        }
        printf("\033[0m\n");

        current_highest_niche = get_highest_niche(niches, n_niches);

        if(current_highest_niche < prev_highest_niche){
            printf("\033[0;31mSolução melhorada!! Usamos o aprimoramento final!\033[0m\n");
            prev_highest_niche = current_highest_niche;
            for(int i = 0; i < n_niches; i++){
                niches_backup[i] = niches[i];
            }
        }else{
            policy_result[curr_policy]->items_status[highest_tower_idx] = prev_status;
            printf("\033[0;31mSolução NÃO FOI melhorada (Rotina de aprimoramento final)!!\033[0m\n");
            for(int i = 0; i < n_niches; i++){
                niches[i] = niches_backup[i];
            }
            break;
        }

    }
}

BestFitResult* best_fit(Data* data){

    BestFitResult* final_result = NULL;

    int packed_items = 0;

    const int n_niches = data->width;


    int niches[n_niches];

    /*All niches have 0 height values in the beginning*/
    for(int i = 0; i < data->width; i++) niches[i] = 0;

    Box pre_processed_items[data->nitems];

    /*Copying the items to a local variable*/
    int item_counter = 0; 
    for(int i = 0; i < data->nitems; i++){

        pre_processed_items[item_counter] = data->items[i];
        if(pre_processed_items[item_counter].height > pre_processed_items[item_counter].width){
            /*Rotate the item if its height is greater than its width*/
            int aux = pre_processed_items[item_counter].height;
            pre_processed_items[item_counter].height = pre_processed_items[item_counter].width;
            pre_processed_items[item_counter].width = aux;
            pre_processed_items[item_counter].rotated = true;
        }
        item_counter++;
    }

    qsort(pre_processed_items, data->nitems, sizeof(Box), compare_boxes);

    printf("All items:\n");
    for(int i = 0; i < data->nitems; i++){
        printf("Item %d - width = %d, height = %d\n", i, pre_processed_items[i].width, pre_processed_items[i].height);
    }


    GapInfo gap_info;
    InsertionInfo insertion_info;
    int x, y, item_width, item_height;
    
    PlacementPolicy policies[] = {LEFTMOST_POLICY, TALLEST_NEIGHBOR_POLICY, SHORTEST_NEIGHBOR_POLICY};
    PlacementPolicy curr_policy = LEFTMOST_POLICY;
    const int last_policy = 2;

    BestFitResult* policy_result[3];

    for(int i = LEFTMOST_POLICY; i <= SHORTEST_NEIGHBOR_POLICY; i++){
        policy_result[i] = (BestFitResult*)malloc(sizeof(BestFitResult));
        policy_result[i]->items_status = (BoxStatus**)malloc(sizeof(BoxStatus*)*data->nunique_items);
        for(int j = 0; j < data->nunique_items; j++){
            policy_result[i]->items_status[j] = (BoxStatus*)malloc(sizeof(BoxStatus));
        }

    }

    int global_highest_niche = INT_MAX;
    int current_highest_niche;
    PlacementPolicy best_policy;

    Box items[data->nitems];

    
    while(curr_policy <= 0){
        packed_items = 0;

        for(int i = 0; i < n_niches; i++){
            niches[i] = 0;
        }

        for(int i = 0; i < data->nitems; i++){
            items[i] = pre_processed_items[i];
        }

        while(packed_items < data->nunique_items){
            gap_info = find_lowest_gap(niches, n_niches);
    
            y = niches[gap_info.index];
            
            insertion_info = find_best_fit_idx(gap_info, items, data->nitems);
    
            if(insertion_info.index == ITEM_NOT_FOUND){
                printf("Item not found...\n");
                ignore_gap(gap_info, niches, n_niches);
                printf("Niches: ");
                for(int i = 0; i < n_niches; i++){
                    printf("%3d ", niches[i]);
                }
                printf("\n");
                continue;
            }
    
            if(insertion_info.rotated != items[insertion_info.index].rotated){
                item_width = items[insertion_info.index].height;
                item_height = items[insertion_info.index].width;
            }else{
                item_width = items[insertion_info.index].width;
                item_height = items[insertion_info.index].height;
            }
    
    
            x = get_x_value(gap_info, policies[curr_policy], item_width, niches, n_niches);
            
            
            items[insertion_info.index].ncopies--;
            printf("\033[0;32minsertion_info.index = %d, packed_items = %d\033[0m\n", insertion_info.index, packed_items);

            printf("Inserindo item(%d, %d) na posição x = %d, y = %d\n", items[insertion_info.index].width, items[insertion_info.index].height, x, y);

            policy_result[curr_policy]->items_status[packed_items]->index = items[insertion_info.index].unique_id;
            policy_result[curr_policy]->items_status[packed_items]->rotated = insertion_info.rotated;
            policy_result[curr_policy]->items_status[packed_items]->x = x;
            policy_result[curr_policy]->items_status[packed_items]->y = y;
    
            printf("Status: id = %d, x = %d, y = %d, rotaded = %d\n", policy_result[curr_policy]->items_status[packed_items]->index, policy_result[curr_policy]->items_status[packed_items]->x, policy_result[curr_policy]->items_status[packed_items]->y, policy_result[curr_policy]->items_status[packed_items]->rotated);

            packed_items++;
            update_niches(niches, gap_info, insertion_info, item_width, item_height);
    
    
            printf("Niches: ");
            for(int i = 0; i < n_niches; i++){
                printf("%3d ", niches[i]);
            }
            printf("\n");
            // getchar();
        }

        printf("<<<<<<<<<<<<<<<<<<<<<< Improvement routine >>>>>>>>>>>>>>>>>>>>>>\n");

        
        reduce_height(data, gap_info, policy_result, curr_policy, niches, n_niches);
    
        current_highest_niche = get_highest_niche(niches, n_niches);

        printf("\033[0;33mAltura da solução: %d\033[0m\n", current_highest_niche);
        if(current_highest_niche < global_highest_niche){
            printf("\033[0;35mNova melhor altura encontrada: %d\033[0m\n", current_highest_niche);
            global_highest_niche = current_highest_niche;
            final_result = policy_result[curr_policy];
            best_policy = curr_policy;
        }

        curr_policy++;
    }



    for(int p = LEFTMOST_POLICY; p <= SHORTEST_NEIGHBOR_POLICY; p++){
        if(p != best_policy){
            for(int i = 0; i < data->nunique_items; i++){
                free(policy_result[p]->items_status[i]);
            }
            free(policy_result[p]->items_status);
            free(policy_result[p]);
        }
    }
    
    

    return final_result;
}