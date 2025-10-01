#include <cmath>
#include <stdexcept>

#include "Core/Error.hpp"

#include "Algorithm/FunctionAST.hpp"
#include "Algorithm/FunctionTokenizer.hpp"

double BinaryOperator::calculate(double x) const {
    double left_value = left_->calculate(x);
    double right_value = right_->calculate(x);

    switch (type_)
    {
    case BinaryOperatorType::PLUS:
        return left_value + right_value;
    case BinaryOperatorType::MINUS:
        return left_value - right_value;
    case BinaryOperatorType::MULTIPLY:
        return left_value * right_value;
    case BinaryOperatorType::DIVISION:
        return left_value * right_value;
    case BinaryOperatorType::POWER:
        return std::pow(left_value, right_value);
        
    default:
        throw std::runtime_error("[ERROR] Try to calculate unsupported binary operator");
        break;
    }
}

double UnaryOperator::calculate(double x) const {
    double next_value = next_->calculate(x);
    switch (type_)
    {
    case UnaryOperatorType::PLUS:
        return next_value;
    case UnaryOperatorType::MINUS:
        return -next_value;
    default:
        throw std::runtime_error("[ERROR] Try to calculate unsupported unary operator");
        break;
    }
}

namespace {
bool is_end(const std::vector<Token>& vector, std::vector<Token>::const_iterator current) {
    return current == vector.end();
}

NodePtr getFactor(const std::vector<Token>& vector, std::vector<Token>::const_iterator& current);
NodePtr getPower(const std::vector<Token>& vector, std::vector<Token>::const_iterator& current); 
NodePtr getPrimary(const std::vector<Token>& vector, std::vector<Token>::const_iterator& current); 

NodePtr getTerm(const std::vector<Token>& vector, std::vector<Token>::const_iterator& current) {
    NodePtr left_side = getFactor(vector, current);

    while (!is_end(vector, current) && current->is_symbol())
    {
        Token::Symbol symbol = current->get_symbol();
        if (symbol != Token::Symbol::MULTIPLY && symbol != Token::Symbol::DIVISION) {
            break;
        } 

        ++current;

        NodePtr right_side = getFactor(vector, current);

        switch (symbol)
        {
        case Token::Symbol::MULTIPLY:
            return std::make_unique<BinaryOperator>(BinaryOperator::BinaryOperatorType::MULTIPLY, std::move(left_side), std::move(right_side));
        case Token::Symbol::DIVISION:
            return std::make_unique<BinaryOperator>(BinaryOperator::BinaryOperatorType::DIVISION, std::move(left_side), std::move(right_side));
        default:
            break;
        }
    }
    
    return left_side;
}

NodePtr getFactor(const std::vector<Token>& vector, std::vector<Token>::const_iterator& current) {
    NodePtr left_side = getPower(vector, current);

    if (!is_end(vector, current) && current->is_symbol() && current->get_symbol() == Token::Symbol::POWER)
    {
        ++current;
        NodePtr right_side = getFactor(vector, current);

        return std::make_unique<BinaryOperator>(BinaryOperator::BinaryOperatorType::POWER, std::move(left_side), std::move(right_side));
    }

    return left_side;
}

NodePtr getPower(const std::vector<Token>& vector, std::vector<Token>::const_iterator& current) {
    if (!is_end(vector, current) && current->is_symbol() && (current->get_symbol() == Token::Symbol::PLUS || current->get_symbol() == Token::Symbol::MINUS))
    {
        Token::Symbol symbol = current->get_symbol();
        ++current;

        NodePtr curr = getPower(vector, current);

        switch (symbol)
        {
        case Token::Symbol::PLUS:
            return std::make_unique<UnaryOperator>(UnaryOperator::UnaryOperatorType::PLUS, std::move(curr));
        case Token::Symbol::MINUS:
            return std::make_unique<UnaryOperator>(UnaryOperator::UnaryOperatorType::MINUS, std::move(curr));
        default:
            break;
        }
    }

    return getPrimary(vector, current);
}

NodePtr getPrimary(const std::vector<Token>& vector, std::vector<Token>::const_iterator& current) {
    if (!is_end(vector, current) && current->is_number()) {
        NodePtr primary = std::make_unique<Number>(current->get_double());
        ++current;
        return primary;
    }
    else if (!is_end(vector, current) && current->is_symbol()) {
        if (current->get_symbol() == Token::Symbol::VARIABLE) {
            NodePtr primary = std::make_unique<Variable>();
            ++current;
            return primary;
        }
        else if (current->get_symbol() == Token::Symbol::LEFT_BRACKET) {
            ++current;
            NodePtr expr = getExpr(vector, current);
            if (current->get_symbol() != Token::Symbol::RIGHT_BRACKET) {
                throw std::runtime_error("[ERROR] Expected ')' in function definition");
            }
            ++current;
            return expr;
        }
    }

    throw std::runtime_error("[ERROR] in getPrimary no one types of tokens");
}
}

NodePtr getExpr(const std::vector<Token>& vector, std::vector<Token>::const_iterator& current) {
    NodePtr left_side = getTerm(vector, current);

    while (!is_end(vector, current) && current->is_symbol())
    {
        Token::Symbol symbol = current->get_symbol();
        if (symbol != Token::Symbol::PLUS && symbol != Token::Symbol::MINUS) {
            break;
        } 

        ++current;

        NodePtr right_side = getTerm(vector, current);

        switch (symbol)
        {
        case Token::Symbol::PLUS:
            return std::make_unique<BinaryOperator>(BinaryOperator::BinaryOperatorType::PLUS, std::move(left_side), std::move(right_side));
        case Token::Symbol::MINUS:
            return std::make_unique<BinaryOperator>(BinaryOperator::BinaryOperatorType::MINUS, std::move(left_side), std::move(right_side));
        default:
            break;
        }
    }
    
    return left_side;
}