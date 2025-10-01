
#include "Core/Error.hpp"
#include "Algorithm/Function.hpp"
#include "Algorithm/FunctionTokenizer.hpp"

Function::Function(const std::string& text, double start, double end) : text_(text), 
                                                                        start_(start), end_(end) {
    std::vector<Token> tokens;
    Token::tokenize(text, tokens);

    // for (auto token : tokens) {
    //     if (token.is_number()) {
    //         ERROR_MSG(token.get_double());
    //     }
    //     if (token.is_symbol()) {
    //         ERROR_MSG(static_cast<unsigned>(token.get_symbol()));
    //     }
    // }

    
}

std::ostream& operator<< (std::ostream& os, const Function& func) {
    os << "Text: " << func.text_ << '\n';
    os << "Start limit: " << func.start_ << '\n';
    os << "End limit: " << func.end_;

    return os;
}