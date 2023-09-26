#include <stdio.h>
#include <string.h>

#include "../libs/alg/lab1/log_calculator.h"

int main()
{
    /*int type = INPUT_TYPE_INVALID;
    while (checkInputType(type) == INPUT_TYPE_INVALID) {
        printf("Select normal form type (0 - disjunctive, 1 - conjunctive): ");
        scanf("%d", &type);
    }
    Formula f = initFormula(type);
    char buf[512];
    gets(buf);
    gets(buf);*/

    Formula f = initFormula(INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM);
    processFormula(&f, "!A + B + A & !C");

    printf("A B C F\n");
    for (int i = 0; i < 8; i++) {
        bool v[LATIN_ALPHABET_LENGTH] = {!!(i & 4), !!(i & 2), !!(i & 1)};
        printf("%d %d %d %d\n", !!v[0], !!v[1], !!v[2], findVal(f, v));
    }

    return 0;
}