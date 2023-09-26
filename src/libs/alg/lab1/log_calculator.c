#include <malloc.h>
#include <string.h>

#include "log_calculator.h"

int checkInputType(int type) {
    if (type == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM)
        return INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM;
    if (type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM)
        return INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM;
    return INPUT_TYPE_INVALID;
}

Formula initFormula(int type) {
    if (checkInputType(type) == INPUT_TYPE_INVALID)
        return (Formula) {NULL, -1, -1, INPUT_TYPE_INVALID};
    return (Formula) {malloc(0), 0, 0, type};
}

void freeFormula(Formula *formula) {
    for (int i = 0; i < formula->amount; i++)
        free(formula->val[i]);

    free(formula->val);

    formula->val = NULL;
    formula->amount = -1;
    formula->capacity = -1;
    formula->type = INPUT_TYPE_INVALID;
}

void addElement(Formula *formula, char element[LATIN_ALPHABET_LENGTH]) {
    if (formula->amount >= formula->capacity)
        formula->val = realloc(formula->val, formula->capacity = (formula->capacity * 2 + 1));

    formula->val[formula->amount++] = malloc(sizeof(char) * LATIN_ALPHABET_LENGTH);
    memcpy(formula->val[formula->amount - 1], element, sizeof(char) * LATIN_ALPHABET_LENGTH);
}

void processFormula(Formula *formula, char* line) {

}