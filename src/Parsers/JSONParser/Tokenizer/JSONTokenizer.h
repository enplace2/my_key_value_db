//
// Created by Nick Place on 3/16/24.
//

#ifndef MY_KEY_VALUE_DB_JSONTOKENIZER_H
#define MY_KEY_VALUE_DB_JSONTOKENIZER_H
#include "vector"
#include "../Structs/Token.h"


class JSONTokenizer {
private:
    TokenVector tokens;
    int cursor;
    std::string inputString;
    Token getNextToken();
    bool hasMoreTokens();
    Token generateKeyToken();
    Token generateStringToken();
public:
    JSONTokenizer(std::string &inputString);
    [[nodiscard]] TokenVector getTokens() const {
        return this->tokens;
    }
    TokenVector generateTokens();
};


#endif //MY_KEY_VALUE_DB_JSONTOKENIZER_H
