#include "SyntaxAnalyzer.h"

int main()
{
    setlocale(LC_ALL, "rus");
    const char* fileName = "ex1.txt";
    // Открытие файла для чтения
    FILE* file;
    errno_t err = fopen_s(&file, fileName, "r");
    if (err != 0 || file == NULL) {
        std::cout << "Can't open file: " << fileName << std::endl;
        return 1;
    }

    
        Scanner *scaner = new Scanner(file);
        SyntaxAnalyzer* diagram = new SyntaxAnalyzer(scaner);

        diagram->program();

        int type; 
        TypeLex lex;
        type = scaner->Scan(lex);
        if (type == typeEnd)
            std::cout << "OK" << std::endl;
        else
            std::cout << "ERROR" << std::endl;
        
    
    /*// Создание объекта Scanner
    Scanner scanner(file);

    // Пример использования метода Scan
    char lexeme[MAX_LEX]; 
    while (true) {
        int result = scanner.Scan(lexeme);
        if (result == typeEnd) {
            break; // Конец файла
        }

        // Обработка результата сканирования
        std::cout << "Lexeme: " << lexeme << "\t type: " << result << std::endl;
    }
    
    fclose(file); // Закрытие файла в конце*/
    system("pause");
    return 0;
}
