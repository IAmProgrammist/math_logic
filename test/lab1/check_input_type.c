#include "../../src/libs/alg/lab1/log_calculator.h"

#include <assert.h>

int main() {
    assert(checkInputType(0) == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM);
    assert(checkInputType(1) == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM);
    assert(checkInputType(99) == INPUT_TYPE_INVALID);
}