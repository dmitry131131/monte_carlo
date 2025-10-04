#pragma once
#include <vector>
#include <memory>
#include "Algorithm/FunctionTokenizer.hpp"

class FunctionNode {
public:
    virtual double calculate(double x) const = 0;
    virtual ~FunctionNode() = default;
};

using NodePtr = std::unique_ptr<FunctionNode>;

class BinaryOperator final : public FunctionNode {
public:
    enum class BinaryOperatorType : unsigned char {
        PLUS,
        MINUS,
        MULTIPLY,
        DIVISION,
        POWER
    };
private:
    BinaryOperatorType type_;
    NodePtr left_;
    NodePtr right_;
public:
    BinaryOperator(BinaryOperatorType type, NodePtr &&left, NodePtr &&right) : 
                                      type_(type), left_(std::move(left)), right_(std::move(right)) {}
    double calculate(double x) const override;

    // Getters
    BinaryOperatorType get_type() {return type_;}
    const FunctionNode& left() const {return *left_;}
    const FunctionNode& right() const {return *right_;}
};

class UnaryOperator final : public FunctionNode {
public:
    enum class UnaryOperatorType : unsigned char {
        PLUS,
        MINUS
    };
private:
    UnaryOperatorType type_;
    NodePtr next_;
public:
    UnaryOperator(UnaryOperatorType type, NodePtr &&next) : type_(type), next_(std::move(next)) {}
    double calculate(double x) const override;

    // Getters
    UnaryOperatorType get_type() {return type_;}
    const FunctionNode& get_next() const {return *next_;}
};

class Number final : public FunctionNode {
    double number_;
public:
    Number(double number) : number_(number) {}
    double calculate(double x) const override {(void) x;  return number_;}
};

class Variable final : public FunctionNode {
public:
    double calculate(double x) const override {return x;}
};

NodePtr getExpr(const std::vector<Token>& vector, std::vector<Token>::const_iterator& current);