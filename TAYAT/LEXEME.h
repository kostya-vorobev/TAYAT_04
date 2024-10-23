#pragma once
#ifndef __LEXEME
#define __LEXEME

#include <string>
using namespace std;

#define MAX_TEXT 10000 // Максимальный размер текста
#define MAX_LEX 100    // Максимальная длина лексемы
#define MAX_NUM 25    // Максимальная длина лексемы
#define MAX_FLT 35    // Максимальная длина лексемы
#define MAX_KEYW 12    // Максимальное количество ключевых слов

typedef char TypeMod[MAX_TEXT];  // Тип для хранения текста (модуля)
typedef char TypeLex[MAX_LEX];   // Тип для хранения лексем

// Коды типов данных
#define typeConst 11 
#define typeShort 12 
#define typeLong 13 
#define typeInt 14 
#define typeDouble 15 
#define typeSwitch 16 
#define typeCase 17 
#define typeBreak 18 
#define typeDefault 19 
#define typeReturn 20 
#define typeClass 21 
#define typeMain 22 

// Коды для идентификаторов и констант
#define typeID 1          // Идентификатор
#define constInt 31      // Целочисленная константа
#define constDouble 32   // Вещественная константа

// Коды для специальных символов
#define typePoint 40     // Точка
#define typeComma 41     // Запятая
#define typeSemicolon 42 // Точка с запятой
#define typeColon 43     // Двоеточие
#define typeLeftBracket 44  // Левый круглый скобка
#define typeRightBracket 45 // Правый круглый скобка
#define typeLeftBrace 46    // Левый фигурный скобка
#define typeRightBrace 47   // Правый фигурный скобка

// Коды операторов
#define typeEval 51      // Оператор равенства
#define typeOr 52        // Логическое ИЛИ
#define typeAnd 53       // Логическое И
#define typeUnEq 54      // Оператор неравенства
#define typeEq 55        // Оператор равенства
#define typeLessOrEq 56  // Оператор меньше или равно
#define typeLess 57      // Оператор меньше
#define typeMoreOrEq 58  // Оператор больше или равно
#define typeMore 59      // Оператор больше
#define typeMinus 60     // Оператор вычитания
#define typePlus 61      // Оператор сложения
#define typeMod 62       // Оператор остатка от деления
#define typeDiv 63       // Оператор деления
#define typeMul 64       // Оператор умножения
#define typeNo 65        // Неизвестный оператор

// Коды для специальных состояний
#define typeEnd 100      // Конец файла
#define typeError 200    // Ошибка при разборе

inline string GetTypeName(int typeCode) {
    switch (typeCode) {
    case typeConst: return "typeConst";
    case typeShort: return "typeShort";
    case typeLong: return "typeLong";
    case typeInt: return "typeInt";
    case typeDouble: return "typeDouble";
    case typeSwitch: return "typeSwitch";
    case typeCase: return "typeCase";
    case typeBreak: return "typeBreak";
    case typeDefault: return "typeDefault";
    case typeReturn: return "typeReturn";
    case typeClass: return "typeClass";
    case typeMain: return "typeMain";
    case typeID: return "typeID";
    case constInt: return "constInt";
    case constDouble: return "constDouble";
    case typePoint: return "typePoint";
    case typeComma: return "typeComma";
    case typeSemicolon: return "typeSemicolon";
    case typeColon: return "typeColon";
    case typeLeftBracket: return "typeLeftBracket";
    case typeRightBracket: return "typeRightBracket";
    case typeLeftBrace: return "typeLeftBrace";
    case typeRightBrace: return "typeRightBrace";
    case typeEval: return "typeEval";
    case typeOr: return "typeOr";
    case typeAnd: return "typeAnd";
    case typeUnEq: return "typeUnEq";
    case typeEq: return "typeEq";
    case typeLessOrEq: return "typeLessOrEq";
    case typeLess: return "typeLess";
    case typeMoreOrEq: return "typeMoreOrEq";
    case typeMore: return "typeMore";
    case typeMinus: return "typeMinus";
    case typePlus: return "typePlus";
    case typeMod: return "typeMod";
    case typeDiv: return "typeDiv";
    case typeMul: return "typeMul";
    case typeNo: return "typeNo";
    case typeEnd: return "typeEnd";
    case typeError: return "typeError";
    default: return "Unknown Type Code";
    }
}

#endif
