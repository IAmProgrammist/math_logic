#include "../../src/libs/alg/lab1/log_calculator.h"

#include <assert.h>

bool formula(bool val[LATIN_ALPHABET_LENGTH])
{
    return (!val[0] || val[1]) && (val[0] + !val[1]);
}

int main()
{
    Formula f = initFormula(INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM);
    processFormula(&f, "(!a + b) & (a + !b)");
    for (int i = 0; i < (2 << 26); i++)
    {
        bool val[LATIN_ALPHABET_LENGTH];

        for (int j = 0; j < LATIN_ALPHABET_LENGTH; j++)
        {
            val[j] = !!(i & (1 << j));
        }

        assert(findVal(f, val) == formula(val));
    }
}