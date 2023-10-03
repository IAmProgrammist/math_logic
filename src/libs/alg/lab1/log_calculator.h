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
    // Элементы формулы, конъюнкты или дизъюнкты в зависимости от type
    char **val;
    // Длина массива элементов
    int amount;
    // Вместимость массива элементов
    int capacity;
    // Тип формулы
    int type;
} Formula;

// Принимает тип формулы type, возвращает INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM, если переданный тип соответствует дизъюнктивной нормальной форме,
// INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM, если переданный тип соответствует конъюнктивной нормальной форме, иначе - INPUT_TYPE_INVALID.
int checkInputType(int type);

// Принимает тип формулы type и на его основе инициализирует и возвращает формулу.
// Если type невалидный, возвращает формулу с типом INPUT_TYPE_INVALID.
Formula initFormula(int type);

// Освобождает из памяти ресурсы, занятые formula
void freeFormula(Formula *formula);

// Добавляет element (конъюнкт или дизъюнкт в зависимости от типа формулы) в formula.
void addElement(Formula *formula, char element[LATIN_ALPHABET_LENGTH]);

// Выполняет парсинг формулы и сохраняет её в formula из строки line.
// Формула в конъюнктивной нормальной форме должна иметь вид "(... + ... + ...) & (... + ... + ... + ...) & (... + ... + ...)"
// Формула в дизъюнктивной нормальной форме должна иметь вид "... & ... & ... + ... & ... & ... & ... + ... & ... & ..."
// Возвращает false, если парсинг был выполнен успешно, иначе - true.
bool processFormula(Formula *formula, char* line);

// Возвращает значение формулы f при значениях переменных из val.
bool findVal(Formula f, bool val[LATIN_ALPHABET_LENGTH]);

#endif //MATH_LOGIC_LOG_CALCULATOR_H
