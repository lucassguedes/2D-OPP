#include <stdio.h>
#include "data.h"
#include "best_fit.h"

int main(int argc, char** argv){
    Data* data = read_instance(argv[1]);

    best_fit(data);

    destroy_data(data);

    return 0;
}