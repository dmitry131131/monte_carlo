#include "Core/Error.hpp"

#include "Algorithm/FunctionTokenizer.hpp"

void Token::tokenize(const std::string& expression, std::vector<Token> &tokens) {
    std::string digit_token;

    for (size_t pos = 0; pos < expression.length(); ++pos) {
        char cur = expression[pos];

        if (std::isspace(cur)) {
            if (!digit_token.empty()) {
                tokens.push_back(std::stod(digit_token));
                digit_token.clear();
            }
            continue;
        }
        
        // If symbol is a token
        auto symbol = symbol_text_.find(cur);
        if (symbol != symbol_text_.end()) {
            if (!digit_token.empty()) {
                tokens.emplace_back(std::stod(digit_token));
                digit_token.clear();
            }
            tokens.emplace_back(cur);
        }
        // If symbol is a digit
        else if (std::isdigit(cur) || cur == '.') {
            digit_token += cur;
        }
    }

    if (!digit_token.empty()) {
        tokens.emplace_back(std::stod(digit_token));
    }
}