#include "CommandParser.h"
#include "../utils/TypeConverter.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>

namespace {
    std::string trim(const std::string& str) {
        size_t start = 0;
        size_t end = str.length();

        while (start < end && std::isspace(str[start])) {
            start++;
        }

        while (end > start && std::isspace(str[end - 1])) {
            end--;
        }

        return str.substr(start, end - start);
    }

    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    std::vector<std::string> tokenize(const std::string& input, std::string& errorMsg) {
        std::vector<std::string> tokens;
        size_t i = 0;
        std::string current;

        while (i < input.length()) {
            if (std::isspace(input[i])) {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                i++;
            } else if (input[i] == '"') {
                i++;
                while (i < input.length() && input[i] != '"') {
                    current += input[i];
                    i++;
                }
                if (i >= input.length()) {
                    errorMsg = "Unclosed quote in command";
                    return {};
                }
                i++;
                tokens.push_back(current);
                current.clear();
            } else {
                current += input[i];
                i++;
            }
        }

        if (!current.empty()) {
            tokens.push_back(current);
        }

        return tokens;
    }

    bool isValidType(const std::string& type) {
        return type == "string" || type == "int" || type == "bool" ||
               type == "double" || type == "uint";
    }
}

ParsedCommand CommandParser::parse(const std::string& input) {
    ParsedCommand result;
    result.type = CommandType::INVALID;

    std::string trimmed = trim(input);
    if (trimmed.empty()) {
        result.errorMsg = "Empty command";
        return result;
    }

    std::string parseError;
    std::vector<std::string> tokens = tokenize(trimmed, parseError);

    if (!parseError.empty()) {
        result.errorMsg = parseError;
        return result;
    }

    if (tokens.empty()) {
        result.errorMsg = "Empty command";
        return result;
    }

    std::string command = toUpper(tokens[0]);

    if (command == "SET") {
        if (tokens.size() != 4) {
            result.errorMsg = "Usage: SET <key> <value> <type>";
            return result;
        }

        if (!isValidType(tokens[3])) {
            result.errorMsg = "Invalid type. Must be one of: string, int, bool, double, uint";
            return result;
        }

        result.type = CommandType::SET;
        result.key = tokens[1];
        result.value = tokens[2];
        result.valueType = tokens[3];

    } else if (command == "GET") {
        if (tokens.size() != 2) {
            result.errorMsg = "Usage: GET <key>";
            return result;
        }

        result.type = CommandType::GET;
        result.key = tokens[1];

    } else if (command == "DELETE") {
        if (tokens.size() != 2) {
            result.errorMsg = "Usage: DELETE <key>";
            return result;
        }

        result.type = CommandType::DELETE;
        result.key = tokens[1];

    } else if (command == "LIST") {
        if (tokens.size() != 1) {
            result.errorMsg = "Usage: LIST";
            return result;
        }

        result.type = CommandType::LIST;

    } else if (command == "EXIT") {
        if (tokens.size() != 1) {
            result.errorMsg = "Usage: EXIT";
            return result;
        }

        result.type = CommandType::EXIT;

    } else if (command == "HELP") {
        if (tokens.size() != 1) {
            result.errorMsg = "Usage: HELP";
            return result;
        }

        result.type = CommandType::HELP;

    } else {
        result.errorMsg = "Unknown command: " + tokens[0];
    }

    return result;
}
