#define _CRT_SECURE_NO_WARNINGS
#include "Scanner.h"

// Конструктор класса Scanner: инициализация данных и установка указателя на начало.
Scanner::Scanner(FILE* in)
{
    GetData(in);            // Получаем данные из файла
    putPointer(0);         // Устанавливаем указатель на начало кода
    lastLineBreakPosition = 0; // Начальная позиция последнего разрыва строки
    currentLine = 0;       // Начальный номер текущей строки
}

// Чтение данных из входного файла
void Scanner::GetData(FILE* in)
{
    if (in == NULL)
    {
        PrintError("Can't open file", ""); // Вывод сообщения об ошибке, если файл не открыт
    }

    int i = 0;
    char tmp;
    // Чтение символов до конца файла
    while (!feof(in))
    {
        fscanf(in, "%c", &tmp); // Чтение одного символа
        if (!feof(in))
            code[i++] = tmp; // Сохранение символа в массив
        if (i > MAX_TEXT)
        {
            PrintError("Error in source file", ""); // Ошибка, если превышен максимальный размер текста
            exit(1); // Завершение программы
        }
    }
    code[i] = '\0'; // Завершение строки
    fclose(in);     // Закрытие файла
}

// Вывод ошибки
void Scanner::PrintError(string errorMessage, string lexeme)
{
    if (lexeme[0] == 0)
        cout << "Error: " << errorMessage << endl; // Вывод ошибки без лексемы
    else
        cout << "Find: " << lexeme << " . Error: " << errorMessage << endl; // Ошибка с лексемой
}

// Макросы для определения различных символов
#define isNumber (code[pointer] <= '9') && (code[pointer] >= '0')
#define isLetterLower (code[pointer] >= 'a') && (code[pointer] <= 'z')
#define isLetterUpper (code[pointer] >= 'A') && (code[pointer] <= 'Z')

// Массив ключевых слов для сканера
TypeLex Keyword[MAX_KEYW] = { "const", "short", "long", "int", "double", "switch", "case",
"break", "default", "return", "class", "main" };

// Индексы для ключевых слов
int IndexKW[MAX_KEYW] = { typeConst, typeShort, typeLong, typeInt, typeDouble,
typeSwitch, typeCase, typeBreak, typeDefault, typeReturn, typeClass, typeMain };

// Основной метод сканирования лексем
int Scanner::Scan(TypeLex lexeme)
{
    int i;
    string errorMessage = "";
    for (int i = 0; i < MAX_LEX; i++)
        lexeme[i] = 0; // Инициализация лексемы
    i = 0;
    bool ignoreSymbols = true; // Флаг игнорирования символов

    // Игнорируем пробельные символы и комментарии
    while (ignoreSymbols)
    {
        while (code[pointer] == ' ' || code[pointer] == '\n' || code[pointer] == '\t')
        {
            if (code[pointer] == '\n')
            {
                currentLine++; // Увеличиваем номер строки при встрече с новой строкой
                lastLineBreakPosition = pointer; // Устанавливаем позицию последнего разрыва строки
            }
            pointer++; // Продвигаем указатель
        }
        // Обработка однострочных комментариев
        if ((code[pointer] == '/') && (code[pointer + 1] == '/'))
        {
            pointer += 2; // Пропускаем символы комментария
            while ((code[pointer] != '\n') && (code[pointer] != '\0'))
                pointer++; // Идем до конца комментария
        }
        else {
            ignoreSymbols = false; // Прекращаем игнорирование символов
        }
    }

    // Обработка конца файла
    if (code[pointer] == '\0')
    {
        lexeme[0] = '#'; // Устанавливаем символ конца
        return typeEnd; // Возвращаем тип окончания
    }

    // Обработка чисел
    if (isNumber)
    {
        lexeme[i++] = code[pointer++]; // Записываем первый символ числа
        while (isNumber)
            if (i < MAX_NUM - 1)
                lexeme[i++] = code[pointer++]; // Записываем оставшиеся цифры
            else
                pointer++; // Пропускаем лишние символы
        // Если превышен максимальный размер числа
        if (i >= MAX_NUM - 1)
        {
            PrintError("Overflow: maximum lexeme size exceeded while processing a number", lexeme);
            for (int i = 0; i < MAX_LEX; i++)
                lexeme[i] = 0; // Сброс лексемы
            i = 0;
            return typeError; // Возвращаем тип ошибки
        }
        // Обработка десятичных чисел
        if (code[pointer] == '.')
        {
            lexeme[i++] = code[pointer++];
            return handleFloatingPoint(lexeme, i); // Обработка плавающей точки
        }
        // Обработка научной нотации
        if ((code[pointer] == 'E') || (code[pointer] == 'e'))
        {
            lexeme[i++] = code[pointer++];
            if (handleExponent(lexeme, i) == constDouble)
                return constDouble; // Возвращаем тип числа с плавающей точкой
        }
        return constInt; // Возвращаем тип целого числа
    }

    // Обработка идентификаторов
    if (isLetterLower || isLetterUpper)
    {
        lexeme[i++] = code[pointer++]; // Записываем первый символ идентификатора
        while (isNumber || isLetterLower || isLetterUpper || code[pointer] == '_')
            if (i < MAX_LEX - 1)
                lexeme[i++] = code[pointer++]; // Записываем оставшиеся символы
            else
                pointer++; // Пропускаем лишние символы
        // Проверка на максимальную длину идентификатора
        if (i >= MAX_LEX - 1)
        {
            PrintError("Overflow: maximum lexeme size exceeded while processing an identifier", lexeme);
            for (int i = 0; i < MAX_LEX; i++)
                lexeme[i] = 0; // Сброс лексемы
            i = 0;
            return typeError; // Возвращаем тип ошибки
        }
        // Проверка идентификатора на ключевое слово
        int j;
        for (j = 0; j < MAX_KEYW; j++)
            if (strcmp(lexeme, Keyword[j]) == 0)
                return IndexKW[j]; // Возвращаем индекс ключевого слова
        return typeID; // Возвращаем тип идентификатора
    }

    // Обработка операторов и символов
    lexeme[i++] = code[pointer++];
    switch (code[pointer - 1])
    {
    case '.':
        if (isNumber)
        {
            lexeme[i++] = code[pointer++];
            return handleFloatingPoint(lexeme, i);
        }
        return typePoint; // Возвращаем тип точки
    case ',':
        return typeComma; // Возвращаем тип запятой
    case ';':
        return typeSemicolon; // Возвращаем тип точки с запятой
    case ':':
        return typeColon; // Возвращаем тип двоеточия
    case '(':
        return typeLeftBracket; // Возвращаем тип левой скобки
    case ')':
        return typeRightBracket; // Возвращаем тип правой скобки
    case '{':
        return typeLeftBrace; // Возвращаем тип левой фигурной скобки
    case '}':
        return typeRightBrace; // Возвращаем тип правой фигурной скобки
    case '%':
        return typeMod; // Возвращаем тип оператора остатка
    case '/':
        return typeDiv; // Возвращаем тип оператора деления
    case '*':
        return typeMul; // Возвращаем тип оператора умножения
    case '-':
        return typeMinus; // Возвращаем тип оператора вычитания
    case '+':
        return typePlus; // Возвращаем тип оператора сложения
    case '=':
        if (code[pointer] == '=')
        {
            lexeme[i++] = code[pointer++];
            return typeEq; // Возвращаем тип оператора равенства
        }
        return typeEval; // Возвращаем тип оператора присваивания
    case '|':
        if (code[pointer] == '|')
        {
            lexeme[i++] = code[pointer++];
            return typeOr; // Возвращаем тип оператора логического ИЛИ
        }
        break;
    case '&':
        if (code[pointer] == '&')
        {
            lexeme[i++] = code[pointer++];
            return typeAnd; // Возвращаем тип оператора логического И
        }
        break;
    case '!':
        if (code[pointer] == '=')
        {
            lexeme[i++] = code[pointer++];
            return typeUnEq; // Возвращаем тип оператора неравенства
        }
        return typeNo; // Возвращаем тип логического отрицания
    case '<':
        if (code[pointer] == '=')
        {
            lexeme[i++] = code[pointer++];
            return typeLessOrEq; // Возвращаем тип оператора меньше или равно
        }
        return typeLess; // Возвращаем тип оператора меньше
    case '>':
        if (code[pointer] == '=')
        {
            lexeme[i++] = code[pointer++];
            return typeMoreOrEq; // Возвращаем тип оператора больше или равно
        }
        return typeMore; // Возвращаем тип оператора больше
    default:
        break; // По умолчанию ничего не делаем
    }

    // Обработка некорректных символов
    errorMessage = "Wrong symbol";
    PrintError(errorMessage, lexeme);
    pointer++; // Продвигаем указатель
    return typeError; // Возвращаем тип ошибки
}

// Обработка чисел с плавающей точкой
int Scanner::parseNumber(TypeLex lexeme, int& i) {
    while (isNumber)
        if (i < MAX_FLT - 1)
            lexeme[i++] = code[pointer++]; // Запись чисел
        else
            pointer++; // Пропуск лишних символов
    if (i >= MAX_FLT - 1)
    {
        PrintError("Overflow: maximum lexeme size exceeded while processing a number", lexeme);
        for (int i = 0; i < MAX_FLT; i++)
            lexeme[i] = 0; // Сброс лексемы
        i = 0;
        return typeError; // Возвращаем тип ошибки
    }
    return constDouble; // Возвращаем тип числа с плавающей точкой
}

// Обработка экспоненты
int Scanner::handleExponent(TypeLex lexeme, int& i) {
    // Обработка знака после 'E'/'e'
    if ((code[pointer] == '+') || (code[pointer] == '-'))
    {
        lexeme[i++] = code[pointer++];
        if (isNumber)
        {
            if (i < MAX_FLT - 1)
                lexeme[i++] = code[pointer++]; // Запись числа после экспоненты
            else
                pointer++;
            if (i >= MAX_FLT - 1)
            {
                PrintError("Overflow: maximum lexeme size exceeded while processing an exponent", lexeme);
                for (int i = 0; i < MAX_FLT; i++)
                    lexeme[i] = 0; // Сброс лексемы
                i = 0;
                return typeError; // Возвращаем тип ошибки
            }
            return parseNumber(lexeme, i); // Парсим число
        }
        else
        {
            PrintError("Wrong const: expected a number after exponent sign", lexeme);
            return typeError; // Ошибка, если после знака экспоненты нет числа
        }
    }
    else
    {
        lexeme[i++] = code[pointer++];
        return parseNumber(lexeme, i); // Парсим число без знака
    }
}

// Обработка чисел с плавающей точкой
int Scanner::handleFloatingPoint(TypeLex lexeme, int& i) {
    while (isNumber)
        if (i < MAX_FLT - 1)
            lexeme[i++] = code[pointer++]; // Запись цифр после точки
        else
            pointer++; // Пропуск лишних символов
    if (i >= MAX_FLT - 1)
    {
        PrintError("Overflow: maximum lexeme size exceeded while processing a floating point number", lexeme);
        for (int i = 0; i < MAX_FLT; i++)
            lexeme[i] = 0; // Сброс лексемы
        i = 0;
        return typeError; // Возвращаем тип ошибки
    }
    // Обработка экспоненты
    if ((code[pointer] == 'E') || (code[pointer] == 'e'))
    {
        lexeme[i++] = code[pointer++];
        return handleExponent(lexeme, i); // Переход к обработке экспоненты
    }
    PrintError("Wrong const: expected 'E' or 'e' for exponential notation", lexeme);
    return typeError; // Ошибка, если нет 'E' или 'e'
}
