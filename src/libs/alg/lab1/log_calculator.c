#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

#include "log_calculator.h"

int checkInputType(int type)
{
    if (type == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM)
        return INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM;
    if (type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM)
        return INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM;
    return INPUT_TYPE_INVALID;
}

Formula initFormula(int type)
{
    if (checkInputType(type) == INPUT_TYPE_INVALID)
        return (Formula){NULL, -1, -1, INPUT_TYPE_INVALID};
    return (Formula){malloc(0), 0, 0, type};
}

void freeFormula(Formula *formula)
{
    for (int i = 0; i < formula->amount; i++)
        free(formula->val[i]);

    free(formula->val);

    formula->val = NULL;
    formula->amount = -1;
    formula->capacity = -1;
    formula->type = INPUT_TYPE_INVALID;
}

void addElement(Formula *formula, char element[LATIN_ALPHABET_LENGTH])
{
    if (formula->amount >= formula->capacity)
        formula->val = realloc(formula->val, formula->capacity = (formula->capacity * 2 + 1));

    formula->val[formula->amount++] = malloc(sizeof(char) * LATIN_ALPHABET_LENGTH);
    memcpy(formula->val[formula->amount - 1], element, sizeof(char) * LATIN_ALPHABET_LENGTH);
}

void processFormula(Formula *formula, char *line)
{
    int type = checkInputType(formula->type);
    if (type == INPUT_TYPE_INVALID)
        return;

    if (type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM)
    {
        return _processFormulaDisjunctive(formula, line);
    }

    if (type == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM)
    {
        return _processFormulaConjunctive(formula, line);
    }
}

static void _processFormulaDisjunctive(Formula *formula, char *line)
{
    // ... & ... & ... + ... & ... & ... & ... + ... & ... & ...

    int bracesCount = 0;
    int presence = LITERAL_UNDEF;
    int letterIndex = -1;
    char element[LATIN_ALPHABET_LENGTH] = {};
    int index = 0;
    while (1)
    {
        char input = *line;
        if (isspace(input))
        {
            // Ignore
        }
        else if (input == NON)
        {
            if (letterIndex != -1)
            {
                fprintf(stderr, "Parsing error at index %d", index);
                return;
            }

            presence = presence == LITERAL_UNDEF ? LITERAL_NEGATIVE : presence == LITERAL_POSITIVE ? LITERAL_NEGATIVE
                                                                                                   : LITERAL_POSITIVE;
        }
        else if (input == CONJUNCTION)
        {
            if (letterIndex == -1)
            {
                fprintf(stderr, "No literal present at index %d", index);
                return;
            }

            element[letterIndex] = presence == LITERAL_UNDEF ? LITERAL_POSITIVE : presence;
            presence = LITERAL_UNDEF;
            letterIndex = -1;
        }
        else if (input == DISJUNCTION)
        {
            if (letterIndex == -1)
            {
                fprintf(stderr, "No literal present at index %d", index);
                return;
            }

            element[letterIndex] = presence == LITERAL_UNDEF ? LITERAL_POSITIVE : presence;
            presence = LITERAL_UNDEF;
            letterIndex = -1;

            if (bracesCount == 0)
            {
                addElement(formula, element);
                memset(element, LITERAL_UNDEF, LATIN_ALPHABET_LENGTH);
            }
            else
            {
                fprintf(stderr, "Brackets error at index %d", index);
                return;
            }
        }
        else if (isupper(input) || islower(input))
        {
            letterIndex = charAlphabetIndex(islower(input) ? toupper(input) : input);
        }
        else if (input == '\0')
        {
            // ... & ... & ... + ... & ... & ... & ... + ... & ... & ...
            if (letterIndex == -1)
            {
                fprintf(stderr, "No literal present at index %d", index);
                return;
            }

            element[letterIndex] = presence == LITERAL_UNDEF ? LITERAL_POSITIVE : presence;

            if (bracesCount == 0)
            {
                addElement(formula, element);
                memset(element, LITERAL_UNDEF, LATIN_ALPHABET_LENGTH);
            }
            else
            {
                fprintf(stderr, "Brackets error at index %d", index);
                return;
            }

            break;
        }
        else
        {
            fprintf(stderr, "Unknown character '%c' at index %d", input, index);
            return;
        }

        line++, index++;
    }
}

static void _processFormulaConjunctive(Formula *formula, char *line)
{
    // (... + ... + ...) & (... + ... + ... + ...) & (... + ... + ...)

    int bracesCount = 0;
    int presence = LITERAL_UNDEF;
    int letterIndex = -1;
    char element[LATIN_ALPHABET_LENGTH] = {};
    int index = 0;
    while (1)
    {
        char input = *line;
        if (isspace(input))
        {
            // Ignore
        }
        else if (input == OPENING_BRACKET)
        {
            if (presence != LITERAL_UNDEF || letterIndex != -1 || bracesCount != 0)
            {
                fprintf(stderr, "Parsing error at index %d", index);
                return;
            }

            bracesCount++;
        }
        else if (input == CLOSING_BRACKET)
        {
            if (letterIndex == -1 || bracesCount != 1)
            {
                fprintf(stderr, "Parsing error at index %d", index);
                return;
            }

            element[letterIndex] = presence == LITERAL_UNDEF ? LITERAL_POSITIVE : presence;
            presence = LITERAL_UNDEF;
            letterIndex = -1;
            bracesCount--;
        }
        else if (input == NON)
        {
            if (letterIndex != -1)
            {
                fprintf(stderr, "Parsing error at index %d", index);
                return;
            }

            presence = presence == LITERAL_UNDEF ? LITERAL_NEGATIVE : presence == LITERAL_POSITIVE ? LITERAL_NEGATIVE
                                                                                                   : LITERAL_POSITIVE;
        }
        else if (input == CONJUNCTION)
        {
            if (bracesCount == 0)
            {
                addElement(formula, element);
                memset(element, LITERAL_UNDEF, LATIN_ALPHABET_LENGTH);
            }
            else
            {
                fprintf(stderr, "Brackets error at index %d", index);
                return;
            }
        }
        else if (input == DISJUNCTION)
        {

            // (... + ... + ...) & (... + ... + ... + ...) & (... + ... + ...)
            if (letterIndex == -1)
            {
                fprintf(stderr, "No literal present at index %d", index);
                return;
            }

            element[letterIndex] = presence == LITERAL_UNDEF ? LITERAL_POSITIVE : presence;
            presence = LITERAL_UNDEF;
            letterIndex = -1;
        }
        else if (isupper(input) || islower(input))
        {
            letterIndex = charAlphabetIndex(islower(input) ? toupper(input) : input);
        }
        else if (input == '\0')
        {
            if (bracesCount == 0)
            {
                addElement(formula, element);
                memset(element, LITERAL_UNDEF, LATIN_ALPHABET_LENGTH);
            }
            else
            {
                fprintf(stderr, "Brackets error at index %d", index);
                return;
            }

            break;
        }
        else
        {
            fprintf(stderr, "Unknown character '%c' at index %d", input, index);
            return;
        }

        line++, index++;
    }
}

bool _findValConjunctive(Formula f, bool val[LATIN_ALPHABET_LENGTH]) {
    bool result = 1;
    for (int i = 0; i < f.amount; i++)
    {
        char *element = f.val[i];
        bool elementResult = 0;

        for (int j = 0; j < LATIN_ALPHABET_LENGTH; j++)
        {
            if (element[j] == LITERAL_UNDEF)
                continue;

            int current = (element[j] == LITERAL_POSITIVE) && val[j];

            if (current)
            {
                elementResult = 1;
                break;
            }
        }

        if (!elementResult)
            return false;
    }

    return result;
}

bool _findValDisjunctive(Formula f, bool val[LATIN_ALPHABET_LENGTH]) {
    for (int i = 0; i < f.amount; i++)
    {
        char *element = f.val[i];
        bool elementResult = true;

        for (int j = 0; j < LATIN_ALPHABET_LENGTH; j++)
        {
            if (element[j] == LITERAL_UNDEF)
                continue;

            int current = (element[j] == LITERAL_POSITIVE) && val[j];
            if (!current)
            {
                elementResult = false;
                break;
            }
        }

        if (elementResult)
            return true;
    }

    return false;
}

bool findVal(Formula formula, bool args[LATIN_ALPHABET_LENGTH])
{
    int type = checkInputType(formula.type);
    if (type == INPUT_TYPE_INVALID)
        return false;

    if (type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM)
    {
        return _findValDisjunctive(formula, args);
    }

    if (type == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM)
    {
        return _findValConjunctive(formula, args);
    }

    return false;

    bool result = type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM ? 0 : 1;
    for (int i = 0; i < f.amount; i++)
    {
        char *element = f.val[i];
        bool elementResult = type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM ? 1 : 0;

        for (int j = 0; j < LATIN_ALPHABET_LENGTH; j++)
        {
            if (element[j] == LITERAL_UNDEF)
                continue;

            int current = element[j] == LITERAL_POSITIVE ? val[j] : !val[j];
            if (type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM && !current)
            {
                elementResult = 0;
                break;
            }
            if (type == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM && current)
            {
                elementResult = 1;
                break;
            }
        }

        if (elementResult && type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM)
            return 1;
        if (!elementResult && type == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM)
            return 0;
    }

    return result;
}