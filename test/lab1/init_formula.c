#include <assert.h>
#include <string.h>
#include "../../src/libs/alg/lab1/log_calculator.h"

int main() {
    Formula formula = initFormula(INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM);
    assert(formula.val != NULL);
    assert(formula.amount == 0);
    assert(formula.capacity == 0);
    assert(formula.type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM);

    Formula formula2 = initFormula(INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM);
    assert(formula2.val != NULL);
    assert(formula2.amount == 0);
    assert(formula2.capacity == 0);
    assert(formula2.type == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM);

    Formula formula3 = initFormula(31293);
    assert(formula3.val == NULL);
    assert(formula3.amount == 0);
    assert(formula3.capacity == 0);
    assert(formula3.type == INPUT_TYPE_INVALID);

    return 0;
}