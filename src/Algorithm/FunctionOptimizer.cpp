#include "Algorithm/Function.hpp"

namespace {
    bool is_const(const FunctionNode* node) {
        switch (node->get_type())
        {
        case FunctionNode::NodeType::Number:
            return true;
        case FunctionNode::NodeType::Variable:
            return false;
        case FunctionNode::NodeType::UnaryOperator:
            return is_const(&static_cast<const UnaryOperator*>(node)->get_next());
        case FunctionNode::NodeType::BinaryOperator:
            return is_const(&static_cast<const BinaryOperator*>(node)->left()) &&
                   is_const(&static_cast<const BinaryOperator*>(node)->right());

        default:
            throw std::runtime_error("Invalid NodeType!"); 
            break;
        }
    }

    bool create_const_opt(FunctionNode* node) {
        switch (node->get_type())
        {
        case FunctionNode::NodeType::BinaryOperator: {
            BinaryOperator* op = static_cast<BinaryOperator*>(node);
            bool is_opt = false;

            if (is_const(&op->left())) {
                op->set_left(std::make_unique<Number>(op->left().calculate(0)));
                is_opt = true;
            }
            else {
                is_opt = create_const_opt(const_cast<FunctionNode*>(&op->left()));
            }
            if (is_const(&op->right())) {
                op->set_right(std::make_unique<Number>(op->right().calculate(0)));
                is_opt = true;
            }
            else {
                is_opt |= create_const_opt(const_cast<FunctionNode*>(&op->right()));
            }
            
            return is_opt;
        }
        case FunctionNode::NodeType::UnaryOperator: {
            UnaryOperator* op = static_cast<UnaryOperator*>(node);
            bool is_opt = false;

            if (is_const(&op->get_next())) {
                op->set_next(std::make_unique<Number>(op->get_next().calculate(0)));
                is_opt = true;
            }
            else {
                is_opt = create_const_opt(const_cast<FunctionNode*>(&op->get_next()));
            }

            return is_opt;
        }

        default:
            return false;
            break;
        }

        return false;
    }
    // TODO create const optimization using graph algorithms  
    bool const_optimization(std::shared_ptr<FunctionNode>& root) {
        if (is_const(root.get())) {
            root = std::make_shared<Number>(root->calculate(0));
            return true;
        }

        return create_const_opt(root.get());
    }
} // namespace

bool Function::optimize() {
    bool optimize = false;

    optimize |= const_optimization(func_);

    return optimize;
}