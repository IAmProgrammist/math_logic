#include <assert.h>
#include <string.h>
#include "../../src/libs/alg/lab1/log_calculator.h"

int main() {
    Formula formula = initFormula(INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM);
    freeFormula(&formula);
    assert(formula.val == NULL);
    assert(formula.amount == 0);
    assert(formula.capacity == 0);
    assert(formula.type == INPUT_TYPE_INVALID);

    return 0;
}