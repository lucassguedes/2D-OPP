#ifndef __BEST_FIT_H__
#define __BEST_FIT_H__
#include "data.h"
#include <limits.h>

/**
 * Description: This  is  a  best-fit  heuristic  for  the  two-dimensional  rectangular 
 * stock-cutting   problem.   This   algorithm   is  based  on  the  following  article: 
 * A New Placement Heuristic for the Orthogonal Stock-Cutting Problem, E. K. Burke et al.
 * 
 * This algorithm was implemented by Lucas Guedes (https://www.github.com/lucassguedes)
 * Last Change: April 17th, 2025.     
 */


#define GAP_NOT_FOUND -1
#define ITEM_NOT_FOUND -1

typedef struct {
    int index; // Index of item
    int x;
    int y;
    bool rotated;
}BoxStatus;

typedef struct {
    bool violated; // Indicates whether the maximum height was violated
    BoxStatus** items_status;
}BestFitResult;

typedef struct {
    int index; // Niche index
    int width;
}GapInfo;

typedef struct {
    int index; // Item index
    bool rotated;
}InsertionInfo;

typedef enum {
    LEFTMOST_POLICY,
    TALLEST_NEIGHBOR_POLICY,
    SHORTEST_NEIGHBOR_POLICY,
}PlacementPolicy;

int            compare_boxes(const void* a, const void* b);
int            get_x_value(GapInfo gap, PlacementPolicy policy, int item_width, int niches[], int n_niches);
int            get_highest_niche(int niches[], int n_niches);

BestFitResult* best_fit(Data* data);
GapInfo        find_lowest_gap(int niches[], int n_niches);
InsertionInfo  find_best_fit_idx(GapInfo gap, Box items[], int nitems);

void           update_niches(int niches[], GapInfo gap, InsertionInfo info, int item_width, int item_height); 
void           ignore_gap(GapInfo gap, int niches[], int n_niches);

#endif