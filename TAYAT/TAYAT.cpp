#include <iostream>
#include "Scanner.h"

int main()
{
    setlocale(LC_ALL, "rus");
    const char* fileName = "ex1.txt";
    // Открытие файла для чтения
    FILE* file;
    errno_t err = fopen_s(&file, fileName, "r");
    if (err != 0 || file == NULL) {
        std::cout << "Не удалось открыть файл: " << fileName << std::endl;
        return 1;
    }

    // Создание объекта Scanner
    Scanner scanner(file);

    // Пример использования метода Scan
    char lexeme[MAX_LEX];  // Изменено на char, чтобы соответствовать типу лексем
    while (true) {
        int result = scanner.Scan(lexeme);
        if (result == typeEnd) {
            break; // Конец файла
        }

        // Обработка результата сканирования
        std::cout << "Лексема: " << lexeme << ", Тип: " << result << std::endl;
    }

    fclose(file); // Закрытие файла в конце
    return 0;
}
