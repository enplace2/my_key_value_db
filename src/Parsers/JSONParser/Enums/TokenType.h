//
// Created by Nick Place on 3/16/24.
//

#ifndef MY_KEY_VALUE_DB_TOKENTYPE_H
#define MY_KEY_VALUE_DB_TOKENTYPE_H
#include "vector"
enum class TokenType {
    KEY,
    COLON = ':',
    STRING,
    BOOLEAN,
    COMMA = ',',
    EOL,
};

const std::vector<TokenType> VALUE_TYPES = {TokenType::STRING, TokenType::BOOLEAN};

#endif //MY_KEY_VALUE_DB_TOKENTYPE_H
