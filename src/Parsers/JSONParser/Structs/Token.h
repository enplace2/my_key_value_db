//
// Created by Nick Place on 3/16/24.
//

#ifndef MY_KEY_VALUE_DB_TOKEN_H
#define MY_KEY_VALUE_DB_TOKEN_H
#include "../Enums/TokenType.h"
#include "string"
#include "vector"
struct Token {
    TokenType type;
    std::string value;
};

using TokenVector = std::vector <Token>;

#endif //MY_KEY_VALUE_DB_TOKEN_H
