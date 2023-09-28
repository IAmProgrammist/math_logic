#include "../../src/libs/alg/lab1/log_calculator.h"

#include <assert.h>

bool formula(bool val[LATIN_ALPHABET_LENGTH])
{
    return (val[15] || val[17] || !val[8] || val[14] || val[16] || !val[4] || val[22] || val[20] || val[3] || !val[7] || !val[2]) && (val[4] || val[7] || !val[20] || val[21] || val[5] || val[25] || val[18] || val[17] || val[3] || val[15] || val[11] || !val[1] || val[9] || !val[2] || !val[8] || !val[22] || val[10] || !val[19] || !val[14] || val[23] || !val[13] || !val[12] || val[6]) && (!val[6] || !val[11] || !val[16] || val[4] || !val[18] || val[24] || !val[15] || !val[5] || val[12] || val[9] || !val[14] || !val[25]) && (val[3] || !val[10] || val[20] || !val[6] || val[8] || val[9] || val[17] || val[13] || !val[5] || !val[21] || !val[11] || val[16] || !val[7] || val[23] || val[14] || val[24] || !val[0] || !val[2]);
}

int main()
{
    Formula f = initFormula(INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM);
    processFormula(&f, "      (     !   !  !    !    !   ! p  +  !   !   R   +   !     !   !      !      ! i  +   !    !    !    !   O +     !   !   Q  +    !     !  !   e   +    !   !   W +     !    !   u   +  D  +    !    !     !      !    !  H +     !      !    !  C       )   &    (     !  !    !    !     !      !   E   + h  +   !     !   !     !  !    !   !    !     !  u  +   !     !    !  !    !  ! V +    !     !     !   !    !    !    !      ! f  +    !   ! Z  +      !    !   !   !    !    !   S  +    !    !  !     !   !    !   r   +   !     !      !    !    !   !    !   ! d   +   !    !    !  !     !   !     !    ! P   +   !   !   L  +    !    !     !   !   ! B  +  J   +  !      !   !    !    !  C +   !   !    !    !     !  !   !  I  +  !     !  !      !   !    !  !   w   +  !    !     !    ! k +     !    !    !  !     !  !    ! T  +    !    !  !    !     !  O  +     !     ! x   +      !   N +     !     !   !   !      !   M  +    !     !    !  !     !    ! g       )   &     (   ! G   +     !      !   !   l +      !    !     !  !      !   q +   !    !    !    !    !   ! e +   !     !    ! S  +     !   !   !   !    !     !   y  +     !     !    !     !  !  P +    !   !  !    !    !     !  ! f   +    !   !    !     !  m  +  !    !  J  +      !   !      !      !  !   O +    !   !    !      !     !  !      !   !     !   z       )   &      (    !    !   D  +    !    !  !    !    !     !   ! K  +   u   +   !    !    !  g +     !     !     !   !      !    !     !   ! i  +   !     !      !    !    !    !  J   +    !     !     !   !     !    !  R   +    !    !    !     ! n +   !  !      !   f  +     !  !     !    !  !     !    !   v +   !      !    !    !   !      !   !  L +   Q  +   !   !    !      !     !  H + X  +    !    !   !     ! O +  y  +     !     !    !      !    !   !  ! a  +    !      !      !    !     !  C   )  ");
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