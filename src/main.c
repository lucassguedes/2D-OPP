#include <stdio.h>
#include "data.h"


int main(int argc, char** argv){
    Data* data = read_instance(argv[1]);


    destroy_data(data);

    return 0;
}