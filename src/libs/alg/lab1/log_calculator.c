#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

#include "log_calculator.h"

int checkInputType(int type)
{
    // Если тип соответствует существующим возможным типам, возращаем этот тип. 
    if (type == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM || type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM)
        return type;

    // Иначе - невалидный тип
    return INPUT_TYPE_INVALID;
}

Formula initFormula(int type)
{
    // Если тип невалидный - возвращаем формулу с ошибочным типом
    if (checkInputType(type) == INPUT_TYPE_INVALID)
        return (Formula){NULL, 0, 0, INPUT_TYPE_INVALID};
    return (Formula){malloc(0), 0, 0, type};
}

void freeFormula(Formula *formula)
{
    // Удаляем каждый элемент формулы
    for (int i = 0; i < formula->amount; i++)
        free(formula->val[i]);

    // Сам массив формул
    free(formula->val);

    // Делаем формулу "невалидной"
    formula->val = NULL;
    formula->amount = 0;
    formula->capacity = 0;
    formula->type = INPUT_TYPE_INVALID;
}

void addElement(Formula *formula, const char element[LATIN_ALPHABET_LENGTH])
{
    // Если количество больше вместимости, увеличиваем размер массива элементов в 2 раза + 1
    if (formula->amount >= formula->capacity)
        formula->val = realloc(formula->val, formula->capacity = (formula->capacity * 2 + 1));

    // Инициализируем массив переменных элемента формулы
    formula->val[formula->amount++] = malloc(sizeof(char) * LATIN_ALPHABET_LENGTH);
    // Копируем переменные в элемент формулы
    memcpy(formula->val[formula->amount - 1], element, sizeof(char) * LATIN_ALPHABET_LENGTH);
}

static bool _processFormulaDisjunctive(Formula *formula, const unsigned char *line)
{
    // ... & ... & ... + ... & ... & ... & ... + ... & ... & ...

    int presence = LITERAL_UNDEF;
    int letterIndex = -1;
    char element[LATIN_ALPHABET_LENGTH] = {0};
    int index = 0;
    while (1)
    {
        char input = *line;
        if (isspace(input))
        {
            // Игнорируем, таким образом пробелы, табуляции и т.д. не будут иметь значения.
        }
        else if (input == NON)
        {
            // Отрицание должно стоять до переменной, а не после.
            if (letterIndex != -1)
            {
                fprintf(stderr, "Parsing error at index %d\n", index);
                return 1;
            }
            
            // Если отрицаний не было, значит переменная стоит с знаком !.
            // Если отрицание было, то !! = отсутствие отрицаний.
            presence = presence == LITERAL_UNDEF ? LITERAL_NEGATIVE : presence == LITERAL_POSITIVE ? LITERAL_NEGATIVE
                                                                                                   : LITERAL_POSITIVE;
        }
        else if (input == CONJUNCTION)
        {
            // Исключение ситуации, подобной "&&".
            if (letterIndex == -1)
            {
                fprintf(stderr, "No literal present at index %d\n", index);
                return 1;
            }

            // Если встречаем &, сохраняем предыдущую переменную в элемент
            element[letterIndex] = presence == LITERAL_UNDEF ? LITERAL_POSITIVE : presence;
            presence = LITERAL_UNDEF;
            letterIndex = -1;
        }
        else if (input == DISJUNCTION)
        {
            // Исключение ситуации, подобной "++".
            if (letterIndex == -1)
            {
                fprintf(stderr, "No literal present at index %d\n", index);
                return 1;
            }

            // Если встречаем +, сохраняем предыдущую переменную в элемент
            element[letterIndex] = presence == LITERAL_UNDEF ? LITERAL_POSITIVE : presence;
            presence = LITERAL_UNDEF;
            letterIndex = -1;
            
            // А также сам элемент в формулу. Также обновляем буфферный элемент element.
            addElement(formula, element);
            memset(element, LITERAL_UNDEF, LATIN_ALPHABET_LENGTH);
            
        }
        else if (isupper(input) || islower(input))
        {
            // Найдена буква латинского алфавита. Выполняем дополнииельные проверки и преобразования так, чтобы 'x' == 'X'.
            letterIndex = charAlphabetIndex(islower(input) ? toupper(input) : input);
        }
        else if (input == '\0')
        {
            // Если дошли до конца строки, сохраняем переменную в элемент, элемент в формулу.
            if (letterIndex == -1)
            {
                fprintf(stderr, "No literal present at index %d\n", index);
                return 1;
            }

            element[letterIndex] = presence == LITERAL_UNDEF ? LITERAL_POSITIVE : presence;

            addElement(formula, element);
            memset(element, LITERAL_UNDEF, LATIN_ALPHABET_LENGTH);

            // Заканчиваем цикл чтения.
            break;
        }
        else
        {
            fprintf(stderr, "Unknown character '%c' at index %d\n", input, index);
            return 1;
        }

        line++, index++;
    }

    return 0;
}

bool _processFormulaConjunctive(Formula *formula, const unsigned char *line)
{
    // (... + ... + ...) & (... + ... + ... + ...) & (... + ... + ...)

    int bracesCount = 0;
    int presence = LITERAL_UNDEF;
    int letterIndex = -1;
    char element[LATIN_ALPHABET_LENGTH] = {0};
    bool elementEmpty = true;
    int index = 0;
    while (1)
    {
        char input = *line;
        if (isspace(input))
        {
            // Игнорируем, таким образом пробелы, табуляции и т.д. не будут иметь значения.
        }
        else if (input == OPENING_BRACKET)
        {
            // Открывающая скобка возможна, если степень вложенности равна 0, она не содержится внутри конъюнкта, конъюнкт пустой. 
            if (presence != LITERAL_UNDEF || letterIndex != -1 || bracesCount != 0 || !elementEmpty)
            {
                fprintf(stderr, "Parsing error at index %d\n", index);
                return 1;
            }

            bracesCount++;
        }
        else if (input == CLOSING_BRACKET)
        {
            // Открывающая скобка возможна, если степень вложенности равна 1, а также конъюнкт не пустой.
            if (letterIndex == -1 || bracesCount != 1)
            {
                fprintf(stderr, "Parsing error at index %d\n", index);
                return 1;
            }

            // Сохраняем переменную в элемент.
            element[letterIndex] = presence == LITERAL_UNDEF ? LITERAL_POSITIVE : presence;
            presence = LITERAL_UNDEF;
            letterIndex = -1;
            bracesCount--;
            elementEmpty = false;
        }
        else if (input == NON)
        {
            // Отрицание должно стоять до переменной, а не после.
            if (letterIndex != -1)
            {
                fprintf(stderr, "Parsing error at index %d\n", index);
                return 1;
            }

            // Если отрицаний не было, значит переменная стоит с знаком !.
            // Если отрицание было, то !! = отсутствие отрицаний.
            presence = presence == LITERAL_UNDEF ? LITERAL_NEGATIVE : presence == LITERAL_POSITIVE ? LITERAL_NEGATIVE
                                                                                                   : LITERAL_POSITIVE;
        }
        else if (input == CONJUNCTION)
        {
            // Встретили &. Если вложенность равна 0, записываем элемент в формулу.
            if (bracesCount == 0)
            {
                addElement(formula, element);
                memset(element, LITERAL_UNDEF, LATIN_ALPHABET_LENGTH);
                elementEmpty = true;
            }
            else
            {
                fprintf(stderr, "Brackets error at index %d\n", index);
                return 1;
            }
        }
        else if (input == DISJUNCTION)
        {
            // Встретили +. Сохраняем переменную в элемент.
            if (letterIndex == -1)
            {
                fprintf(stderr, "No literal present at index %d\n", index);
                return 1;
            }

            element[letterIndex] = presence == LITERAL_UNDEF ? LITERAL_POSITIVE : presence;
            presence = LITERAL_UNDEF;
            letterIndex = -1;
            elementEmpty = false;
        }
        else if (isupper(input) || islower(input))
        {
            // Найдена буква латинского алфавита. Выполняем дополнииельные проверки и преобразования так, чтобы 'x' == 'X'.
            letterIndex = charAlphabetIndex(islower(input) ? toupper(input) : input);
        }
        else if (input == '\0')
        {
            // Дошли до конца строки. Записываем элемент в формулу. Проверяем, что степень вложенности == 0.
            if (bracesCount == 0)
            {
                addElement(formula, element);
                memset(element, LITERAL_UNDEF, LATIN_ALPHABET_LENGTH);
            }
            else
            {
                fprintf(stderr, "Brackets error at index %d\n", index);
                return 1;
            }

            // Заканчиваем цикл чтения.
            break;
        }
        else
        {
            fprintf(stderr, "Unknown character '%c' at index %d\n", input, index);
            return 1;
        }

        line++, index++;
    }

    return 0;
}

bool processFormula(Formula *formula, const unsigned char *line)
{
    // В зависимости от типа формулы выполняем парсинг при помощи соответствующей функции.
    int type = checkInputType(formula->type);
    if (type == INPUT_TYPE_INVALID)
        return 1;

    if (type == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM)
    {
        return _processFormulaDisjunctive(formula, line);
    }

    if (type == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM)
    {
        return _processFormulaConjunctive(formula, line);
    }
}

static bool _findValConjunctive(Formula f, const bool val[LATIN_ALPHABET_LENGTH]) {
    // (... + ... + ...) & (... + ... + ... + ...) & (... + ... + ...)
    // Вычисления выполняются по ленивой схеме.
    
    for (int i = 0; i < f.amount; i++)
    {
        char *element = f.val[i];
        // Предположим, что результат элемента ... + ... + ... - false.
        bool elementResult = false;

        for (int j = 0; j < LATIN_ALPHABET_LENGTH; j++)
        {
            // Если переменной нет в формуле, пропускаем её.
            if (element[j] == LITERAL_UNDEF)
                continue;

            // Вычисляем значение элемента.
            int current = (element[j] == LITERAL_POSITIVE) ? val[j] : !val[j];

            // Если значение переменной true - присваиваем всему элементу true и заканчиваем цикл, дальнейшее вычисление не нужно.
            if (current)
            {
                elementResult = true;
                break;
            }
        }

        // Если элемент формулы false, то формула вида ... & ... & ... тоже будет иметь результат false 
        if (!elementResult)
            return false;
    }

    // Все элементы ... & ... & ... true, формула тоже true.
    return true;
}

static bool _findValDisjunctive(Formula f, const bool val[LATIN_ALPHABET_LENGTH]) {
    // ... & ... & ... + ... & ... & ... & ... + ... & ... & ...
    // Вычисления выполняются по ленивой схеме.
    for (int i = 0; i < f.amount; i++)
    {
        char *element = f.val[i];
        // Предположим, что результат элемента ... & ... & ... - true.
        bool elementResult = true;

        for (int j = 0; j < LATIN_ALPHABET_LENGTH; j++)
        {
            // Если переменной нет в формуле, пропускаем её.
            if (element[j] == LITERAL_UNDEF)
                continue;

            // Вычисляем значение элемента.
            int current = (element[j] == LITERAL_POSITIVE) ? val[j] : !val[j];
            // Если значение переменной false - присваиваем всему элементу false и заканчиваем цикл, дальнейшее вычисление не нужно.
            if (!current)
            {
                elementResult = false;
                break;
            }
        }

        // Если элемент формулы true, то формула вида ... + ... + ... тоже будет иметь результат true 
        if (elementResult)
            return true;
    }

    // Все элементы оказались false, возвращаем false.
    return false;
}

bool findVal(Formula formula, const bool args[LATIN_ALPHABET_LENGTH])
{
    // В зависимости от типа формулы выполняем вычисление значения формулы.
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
}