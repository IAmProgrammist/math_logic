#include <stdio.h>
#include "../libs/alg/lab1/log_calculator.h"

int main() {
    int type = INPUT_TYPE_INVALID;
    while (checkInputType(type) == INPUT_TYPE_INVALID) {
        printf("Select normal form type (0 - disjunctive, 1 - conjunctive): ");
        scanf("%d", &type);
    }



    return 0;
}