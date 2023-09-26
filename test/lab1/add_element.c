#include <assert.h>
#include <string.h>
#include "../../src/libs/alg/lab1/log_calculator.h"

int main() {
    Formula formula = initFormula(INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM);
    char element[LATIN_ALPHABET_LENGTH] = {LITERAL_POSITIVE, LITERAL_NEGATIVE, 99, LITERAL_NEGATIVE};

    addElement(&formula, element);
    addElement(&formula, element);

    assert(formula.amount == 2);
    assert(formula.capacity >= formula.amount);
    assert(!strncmp(formula.val[0], element, LATIN_ALPHABET_LENGTH * sizeof(char)));
    assert(!strncmp(formula.val[1], element, LATIN_ALPHABET_LENGTH * sizeof(char)));

    return 0;
}