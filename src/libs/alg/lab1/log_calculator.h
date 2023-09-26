#ifndef MATH_LOGIC_LOG_CALCULATOR_H
#define MATH_LOGIC_LOG_CALCULATOR_H

#include <stdbool.h>

#define INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM 0
#define INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM 1
#define INPUT_TYPE_INVALID (-1)

#define LITERAL_POSITIVE 1
#define LITERAL_UNDEF 0
#define LITERAL_NEGATIVE (-1)

#define LATIN_ALPHABET_LENGTH 26

#define NON '!'
#define CONJUNCTION '&' // Стрелочка вверх
#define DISJUNCTION '+' // Стрелочка вниз
#define OPENING_BRACKET '('
#define CLOSING_BRACKET ')'
#define charAlphabetIndex(val) ((val) - 'A')

typedef struct Formula {
    char **val;
    int amount;
    int capacity;
    int type;
} Formula;

int checkInputType(int type);

Formula initFormula(int type);

void freeFormula(Formula *formula);

void addElement(Formula *formula, char element[LATIN_ALPHABET_LENGTH]);

void processFormula(Formula *formula, char* line);

bool findVal(Formula f, bool val[LATIN_ALPHABET_LENGTH]);

#endif //MATH_LOGIC_LOG_CALCULATOR_H
