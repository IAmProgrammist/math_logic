#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../libs/alg/lab1/log_calculator.h"

int main()
{
    int type = INPUT_TYPE_INVALID;
    while (checkInputType(type) == INPUT_TYPE_INVALID) {
        printf("Select normal form type (0 - disjunctive, 1 - conjunctive): ");
        scanf("%d", &type);
    }

    char buf[512];
    gets(buf);
    Formula f;
    do {
    printf("Enter formula: ");
        f = initFormula(type);

        gets(buf);
    } while(processFormula(&f, buf));

    bool isPresent[LATIN_ALPHABET_LENGTH] = {};
    for (int i = 0; buf[i] != '\0'; i++) {
        if (islower(buf[i]))
            isPresent[buf[i] - 'a'] = true;
        if (isupper(buf[i]))
            isPresent[buf[i] - 'A'] = true;
    }

    printf("Enter variables:\n");
    for (int i = 0; i < LATIN_ALPHABET_LENGTH; i++) {
        if (!isPresent[i]) continue;

        printf("%c ", i + 'A');
    }
    printf("\n");

    bool v[LATIN_ALPHABET_LENGTH];
    for (int i = 0; i < LATIN_ALPHABET_LENGTH; i++) {
        if (!isPresent[i]) continue;
        
        scanf("%d", v + i);
    }
        
    printf("Result: %d", findVal(f, v));

    return 0;
}