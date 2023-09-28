#include "../../src/libs/alg/lab1/log_calculator.h"

#include <assert.h>

bool formula(bool val[LATIN_ALPHABET_LENGTH])
{
    return !val[7] && val[12] && val[19] && val[1] && val[5] && val[10] && !val[8] && val[17] && val[23] && val[3] && !val[20] && !val[4] && val[25] && val[2] || val[6] && !val[16] && val[22] && !val[12] && !val[25] && !val[5] || !val[17] && val[18] && val[1] && val[11] && val[21] && !val[2] && val[7] && !val[4] && !val[13] && !val[3] && val[8] && !val[15] && val[9] && !val[23] && !val[22] && val[12] && val[0] && val[10] && val[24] && val[5] && val[25] && !val[19] && val[20] && !val[6] || val[15] && !val[24] && val[5] && !val[21] && !val[18] && !val[12] && !val[11] && val[23] && !val[14] && val[0] && val[16];
}

//! H&M&T&B&F&K&!I&R&X&D&!U&!E&Z&C + G&!Q&W&!M&!Z&!F + !R&S&B&L&V&!C&H&!E&!N&!D&I&!P&J&!X&!W&M&A&K&Y&F&Z&!T&U&!G + P&!Y&F&!V&!S&!M&!L&X&!O&A&Q
int main()
{
    Formula f = initFormula(INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM);
    processFormula(&f, "  !      !    !   !     !      !      !   H &   !  !    !  !   !    !    !   ! M &    !   !    !     !    !    !  T &   !    !     !  ! b & F  &    !    !   !     !     !   !  K  &      !    !   !     !   !   !   !  i   &   !     !   R   & x  & d  &   !   !      !     !    !  !    !    !     ! U  &     !  e & z &   !      !   C      +    !     !      !    !  G &      !    !      !     !     !      !      !    !   !  Q  &   !    !   !  !     !   !    !  !  w   &     !    !  !    !   !     !    !      !      ! M &    !   !      !  !     !    !     !  Z   &    !  !     !  !   !    !     !   !  !   F       +      !     !     ! r &  !  !    !     !  S  &   !    !     !    !    !   !  b &    !    !  l  &    !  !  !    !    !     ! v &    !     !     !  !    !   !      ! c   &  !   !  !     ! H  &    !   e &      !    !    !     !     !     !    !   n &   !      !    !      !   !   !    !    !    ! D  &   !   !    !    ! I   &     !     !   !     !  !    !    !   p   &  !    !    !    !    !     !   J   &      !     !    !   !    !  !    !    !    !   x &    !   !   !     !     !     !     !   !    ! w &   !     !   !      !   M   &     !   !  A &     !  ! k   & y &    !     !   !    !     !    !  !   ! f   &     !     !    !   !   Z  &   !      !   !   !    !  !    !   !    ! T   &   u &   !   g       +     !  !    !     !     !   !    !     ! p   &    !     !    !  !   !   !      !  !  !  Y   &    !     !     !      !     !   !      !     ! f   &     !  !     !   V   &  !  S  &  !    !   !   !   !   !    !      !    !  m   &   !    !    !    !    !  l  &     !    !  x &     !  O   &   A &     !   !     !   !    !     !     !  !   Q           ");
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