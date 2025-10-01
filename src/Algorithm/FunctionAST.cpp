#include <cmath>
#include <stdexcept>
#include "Algorithm/FunctionAST.hpp"
#include "Algorithm/FunctionTokenizer.hpp"

double BinaryOperator::calculate(double x) {
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
        return std::pow(left_value, right_value);
        
    default:
        throw std::runtime_error("[ERROR] Try to calculate unsupported binary operator");
        break;
    }
}

double UnaryOperator::calculate(double x) {
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
    NodePtr getFactor(std::vector<Token>::const_iterator& current);
    NodePtr getPower(std::vector<Token>::const_iterator& current); 
    NodePtr getPrimary(std::vector<Token>::const_iterator& current); 

    NodePtr getTerm(std::vector<Token>::const_iterator& current) {
        NodePtr left_side = getFactor(current);

        while (current->is_symbol())
        {
            Token::Symbol symbol = current->get_symbol();
            ++current;

            if (symbol != Token::Symbol::MULTIPLY && symbol != Token::Symbol::DIVISION) {
                continue;
            } 

            NodePtr right_side = getFactor(current);

            switch (symbol)
            {
            case Token::Symbol::MULTIPLY:
                return std::make_unique<BinaryOperator>(BinaryOperator::BinaryOperatorType::MULTIPLY, left_side, right_side);
            case Token::Symbol::DIVISION:
                return std::make_unique<BinaryOperator>(BinaryOperator::BinaryOperatorType::DIVISION, left_side, right_side);
            default:
                break;
            }
        }
        
        return left_side;
    }

    NodePtr getFactor(std::vector<Token>::const_iterator& current) {
        NodePtr left_side = getPower(current);

        if (current->is_symbol() && current->get_symbol() == Token::Symbol::POWER)
        {
            ++current;
            NodePtr right_side = getFactor(current);

            return std::make_unique<BinaryOperator>(BinaryOperator::BinaryOperatorType::POWER, left_side, right_side);
        }
        
        return left_side;
    }

    NodePtr getPower(std::vector<Token>::const_iterator& current) {
        if (current->is_symbol() && (current->get_symbol() == Token::Symbol::PLUS || current->get_symbol() == Token::Symbol::MINUS))
        {
            Token::Symbol symbol = current->get_symbol();
            ++current;

            NodePtr curr = getPower(current);

            switch (symbol)
            {
            case Token::Symbol::PLUS:
                return std::make_unique<UnaryOperator>(UnaryOperator::UnaryOperatorType::PLUS, curr);
            case Token::Symbol::MINUS:
                return std::make_unique<UnaryOperator>(UnaryOperator::UnaryOperatorType::MINUS, curr);
            default:
                break;
            }
        }

        return getPrimary(current);
    }

    NodePtr getPrimary(std::vector<Token>::const_iterator& current) {
        if (current->is_number()) {
            return std::make_unique<Number>(current->get_double());
        }
        else if (current->is_symbol()) {
            if (current->get_symbol() == Token::Symbol::VARIABLE) {
                return std::make_unique<Variable>();
            }
            else if (current->get_symbol() == Token::Symbol::LEFT_BRACKET) {
                ++current;
                NodePtr expr = getExpr(current);
                if (current->get_symbol() != Token::Symbol::RIGHT_BRACKET) {
                    throw std::runtime_error("[ERROR] Expected ')' in function definition");
                }
                return expr;
            }
        }

        ++current;
    }
}

NodePtr getExpr(std::vector<Token>::const_iterator current) {
    NodePtr left_side = getTerm(current);

    while (current->is_symbol())
    {
        Token::Symbol symbol = current->get_symbol();
        ++current;

        if (symbol != Token::Symbol::PLUS && symbol != Token::Symbol::MINUS) {
            continue;
        } 

        NodePtr right_side = getTerm(current);

        switch (symbol)
        {
        case Token::Symbol::PLUS:
            return std::make_unique<BinaryOperator>(BinaryOperator::BinaryOperatorType::PLUS, left_side, right_side);
        case Token::Symbol::MINUS:
            return std::make_unique<BinaryOperator>(BinaryOperator::BinaryOperatorType::MINUS, left_side, right_side);
        default:
            break;
        }
    }
    
    return left_side;
}