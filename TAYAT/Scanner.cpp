#define _CRT_SECURE_NO_WARNINGS
#include "Scanner.h"

Scanner::Scanner(FILE* in) {
    GetData(in);
    putPointer(0);
    lastLineBreakPosition = 0;
    currentLine = 0;
}

// Считывание данных из файла
void Scanner::GetData(FILE* in) {
    if (in == NULL) {
        PrintError("Can't open file", "");
    }
    int i = 0;
    char tmp;

    // Чтение файла по символам
    while (!feof(in)) {
        fscanf(in, "%c", &tmp);
        if (!feof(in)) {
            code[i++] = tmp;
        }
        if (i > MAX_TEXT) {
            PrintError("Error in source file", "");
            exit(1);
        }
    }
    code[i] = '\0'; // Завершение строки
    fclose(in); // Закрытие файла
}

// Вывод сообщения об ошибке
void Scanner::PrintError(string errorMessage, string lexeme) {
    if (lexeme[0] == 0) {
        cout << "Error: " << errorMessage << endl;
    }
    else {
        cout << "Find: " << lexeme << " . Error: " << errorMessage << endl;
    }
    cout << "Нераспознанный символ: " << code[pointer - 1] << endl; // Информация о текущем символе
    exit(1); // Завершение программы при ошибке
}

// Макросы для проверки символов
#define isNumber (code[pointer] <= '9') && (code[pointer] >= '0')
#define isLetterLower (code[pointer] >= 'a') && (code[pointer] <= 'z')
#define isLetterUpper (code[pointer] >= 'A') && (code[pointer] <= 'Z')

// Определение ключевых слов
TypeLex Keyword[MAX_KEYW] = {
    "const", "short", "long", "int", "double", "switch",
    "case", "break", "default", "return", "class", "main"
};

// Индексы ключевых слов
int IndexKW[MAX_KEYW] = {
    typeConst, typeShort, typeLong, typeInt, typeDouble,
    typeSwitch, typeCase, typeBreak, typeDefault, typeReturn,
    typeClass, typeMain
};

// Основной метод сканирования
int Scanner::Scan(TypeLex lexeme) {
    int i;
    for (int j = 0; j < MAX_LEX; j++)
        lexeme[j] = 0; // Инициализация лексемы

    i = 0;
    bool ignoreSymbols = true;

    // Игнорирование пробелов и комментариев
    while (ignoreSymbols) {
        while (code[pointer] == ' ' || code[pointer] == '\n' || code[pointer] == '\t') {
            if (code[pointer] == '\n') {
                currentLine++;
                lastLineBreakPosition = pointer; // Обновление позиции последнего разрыва строки
            }
            pointer++;
        }
        if ((code[pointer] == '/') && (code[pointer + 1] == '/')) {
            pointer += 2; // Пропуск комментариев
            while ((code[pointer] != '\n') && (code[pointer] != '\0'))
                pointer++;
        }
        else {
            ignoreSymbols = false; // Выход из цикла при нахождении символа
        }
    }

    // Обработка конца файла
    if (code[pointer] == '\0') {
        lexeme[0] = '#';
        return typeEnd; // Возврат типа конца
    }

    // Обработка целых чисел
    if (isNumber) {
        // Чтение целого числа
        do {
            if (i < MAX_LEX - 1) {
                lexeme[i++] = code[pointer++];
            }
            else {
                PrintError("Int literal too long", lexeme);
                return typeError; // Возврат ошибки
            }
        } while (isNumber);

        // Проверка на вещественное число
        if (code[pointer] == '.') {
            if (i == 0) { // '.' не может быть первым символом
                PrintError("Invalid float format", lexeme);
                return typeError;
            }
            lexeme[i++] = code[pointer++]; // добавляем точку

            // Считываем дробную часть
            while (isNumber) {
                if (i < MAX_LEX - 1) {
                    lexeme[i++] = code[pointer++];
                }
                else {
                    PrintError("Double literal too long", lexeme);
                    return typeError; // Возврат ошибки
                }
            }

            // Проверка на экспоненциальный формат
            if (code[pointer] == 'E' || code[pointer] == 'e') {
                lexeme[i++] = code[pointer++]; // добавляем символ экспоненты

                // Обработка знака экспоненты
                if (code[pointer] == '+' || code[pointer] == '-') {
                    lexeme[i++] = code[pointer++];
                }

                // Считываем целое число для экспоненты
                while (isNumber) {
                    if (i < MAX_LEX - 1) {
                        lexeme[i++] = code[pointer++];
                    }
                    else {
                        PrintError("Double literal too long", lexeme);
                        return typeError; // Возврат ошибки
                    }
                }
            }

            lexeme[i] = '\0'; // Завершение строки
            return constDouble; // Возврат вещественного числа
        }

        lexeme[i] = '\0'; // Завершение строки
        return constInt; // Возврат целого числа
    }

    // Обработка идентификаторов и ключевых слов
    if (isLetterLower || isLetterUpper) {
        do {
            if (i < MAX_LEX - 1) lexeme[i++] = code[pointer++];
        } while (isNumber || isLetterLower || isLetterUpper || code[pointer] == '_');

        lexeme[i] = '\0'; // Завершение идентификатора

        // Определяем, является ли идентификатор ключевым словом
        for (int j = 0; j < MAX_KEYW; j++) {
            if (strcmp(lexeme, Keyword[j]) == 0) {
                return IndexKW[j]; // Возвращаем соответствующий тип ключевого слова
            }
        }

        return typeID; // Возврат идентификатора
    }

    // Обработка специальных символов и знаков операций
    switch (code[pointer]) {
    case '.': lexeme[i++] = code[pointer++]; return typePoint;
    case ',': lexeme[i++] = code[pointer++]; return typeComma;
    case ';': lexeme[i++] = code[pointer++]; return typeSemicolon;
    case ':': lexeme[i++] = code[pointer++]; return typeColon;
    case '(': lexeme[i++] = code[pointer++]; return typeLeftBracket;
    case ')': lexeme[i++] = code[pointer++]; return typeRightBracket;
    case '{': lexeme[i++] = code[pointer++]; return typeLeftBrace;
    case '}': lexeme[i++] = code[pointer++]; return typeRightBrace;
    case '[': lexeme[i++] = code[pointer++]; return typeLeftSqBracket;
    case ']': lexeme[i++] = code[pointer++]; return typeRightSqBracket;

    case '=':
        lexeme[i++] = code[pointer++];
        if (code[pointer] == '=')
            lexeme[i++] = code[pointer++];
        return typeEval;

    case '%': lexeme[i++] = code[pointer++]; return typeMod;
    case '/': lexeme[i++] = code[pointer++]; return typeDiv;
    case '*': lexeme[i++] = code[pointer++]; return typeMul;
    case '-': lexeme[i++] = code[pointer++]; return typeMinus;
    case '+': lexeme[i++] = code[pointer++]; return typePlus;

    case '<':
        lexeme[i++] = code[pointer++];
        if (code[pointer] == '=') {
            lexeme[i++] = code[pointer++];
            return typeLessOrEq;
        }
        return typeLess;

    case '>':
        lexeme[i++] = code[pointer++];
        if (code[pointer] == '=') {
            lexeme[i++] = code[pointer++];
            return typeMoreOrEq;
        }
        return typeMore;

    case '!':
        lexeme[i++] = code[pointer++];
        if (code[pointer] == '=') {
            lexeme[i++] = code[pointer++];
            return typeUnEq;
        }
        return typeNo;

    case '|':
        lexeme[i++] = code[pointer++];
        if (code[pointer] == '|') {
            lexeme[i++] = code[pointer++];
            return typeOr;
        }
        break;

    case '&':
        lexeme[i++] = code[pointer++];
        if (code[pointer] == '&') {
            lexeme[i++] = code[pointer++];
            return typeAnd;
        }
        break;

    default:
        lexeme[i++] = code[pointer++];
        PrintError("Wrong symbol", lexeme);
        return typeError;
    }

    return typeError; // Возврат ошибки при нераспознании токена
}
