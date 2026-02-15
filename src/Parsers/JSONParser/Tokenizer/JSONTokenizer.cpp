//
// Created by Nick Place on 3/16/24.
//

#include "JSONTokenizer.h"

JSONTokenizer::JSONTokenizer(std::string &inputString) {
    this->inputString = inputString;
    this->cursor = 0;
}

TokenVector JSONTokenizer::generateTokens() {
    return this->tokens;
}

Token JSONTokenizer::getNextToken() {
    Token token;
    if(!hasMoreTokens()){
        token = {
                TokenType::EOL,
                ""
        };
    }
    if(this->inputString[cursor] == '"'){
        token = generateStringToken();
    }
    return token;
}

bool JSONTokenizer::hasMoreTokens() {
    return cursor != this->inputString.length() -1;
}

Token JSONTokenizer::generateKeyToken() {
    std::string key;
    while(inputString[cursor] != ':'){
        key+=inputString[cursor];
        cursor++;
    }
    return {
        TokenType::KEY,
        key
    };
}

Token JSONTokenizer::generateStringToken() {
    //move past opening quotes
    cursor++;
    std::string value;
    while(this->inputString[cursor] != '"'){
        if(cursor == this->inputString.length()-1){
            throw std::runtime_error("Unexpected end of input while parsing a string. Missing closing quotations ");
        }
        value += this->inputString[cursor];
        cursor++;
    }
    //move past closing quotes
    cursor++;
    return {
        TokenType::STRING,
        value
    };
}
