#ifndef MY_KEY_VALUE_DB_COMMANDPARSER_H
#define MY_KEY_VALUE_DB_COMMANDPARSER_H

#include <string>

enum class CommandType {
    SET,
    GET,
    DELETE,
    LIST,
    EXIT,
    HELP,
    INVALID
};

struct ParsedCommand {
    CommandType type;
    std::string key;
    std::string value;
    std::string valueType;
    std::string errorMsg;
};

class CommandParser {
public:
    static ParsedCommand parse(const std::string& input);
};

#endif
