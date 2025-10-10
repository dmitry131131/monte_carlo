
#include "Core/Error.hpp"
#include "Algorithm/Function.hpp"
#include "Algorithm/FunctionTokenizer.hpp"
#include "Algorithm/FunctionAST.hpp"

#include "Dumper/FunctionDumper.hpp"

Function::Function(const std::string& text, double start, double end) : settings_({text, start, end}) {
    std::vector<Token> tokens;
    Token::tokenize(text, tokens);

    auto first_token = tokens.cbegin();
    func_ = getExpr(tokens, first_token);
}

std::ostream& operator<< (std::ostream& os, const Function& func) {
    os << func.get_settings();

    return os;
}

std::ostream& operator<< (std::ostream& os, const Function::Settings& func_settings) {
    os << "Text: " << func_settings.text_ << '\n';
    os << "Start limit: " << func_settings.start_ << '\n';
    os << "End limit: " << func_settings.end_;

    return os;
}