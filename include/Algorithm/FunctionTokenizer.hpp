#pragma once
#include <variant>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <vector>

class Token final {
public:
    enum class Symbol : unsigned char {
        PLUS,
        MINUS,
        MULTIPLY,
        DIVISION,
        POWER,
        VARIABLE,
        LEFT_BRACKET,
        RIGHT_BRACKET
    };
private:
    inline static const std::unordered_map<char, Symbol> symbol_text_{
        {'+', Symbol::PLUS},
        {'-', Symbol::MINUS},
        {'*', Symbol::MULTIPLY},
        {'/', Symbol::DIVISION},
        {'^', Symbol::POWER},
        {'x', Symbol::VARIABLE},
        {'(', Symbol::LEFT_BRACKET},
        {')', Symbol::RIGHT_BRACKET}
    };

    using TokenType = std::variant<double, Symbol>;
    TokenType token_;
public:
    Token(double number) : token_(number) {}
    Token(char text) {
        auto symbol = symbol_text_.find(text);
        if (symbol == symbol_text_.end()) {
            throw std::runtime_error("[ERROR] No such symbol if function");
        }
        token_ = TokenType{symbol->second};
    }

    bool is_number() const {return std::holds_alternative<double>(token_);}
    bool is_symbol() const {return std::holds_alternative<Symbol>(token_);}

    double get_double() const {return std::get<double>(token_);}
    Symbol get_symbol() const {return std::get<Symbol>(token_);}

    static void tokenize(const std::string& expression, std::vector<Token> &tokens);
};
