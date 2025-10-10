#include "Dumper/FunctionDumper.hpp"

namespace {
    void add_record(FunctionDumper::nodes_vector& nodes, FunctionDumper::edges_vector& edges, const FunctionNode *node) {
        switch (node->get_type())
        {
        case FunctionNode::NodeType::UnaryOperator:
            nodes.push_back(node);
            edges.push_back({node, &static_cast<const UnaryOperator*>(node)->get_next()});
            add_record(nodes, edges, &static_cast<const UnaryOperator*>(node)->get_next());
            break;
        case FunctionNode::NodeType::BinaryOperator:
            nodes.push_back(node);
            edges.push_back({node, &static_cast<const BinaryOperator*>(node)->left()});
            edges.push_back({node, &static_cast<const BinaryOperator*>(node)->right()});
            add_record(nodes, edges, &static_cast<const BinaryOperator*>(node)->left());
            add_record(nodes, edges, &static_cast<const BinaryOperator*>(node)->right());
            break;
        
        default:
            nodes.push_back(node);
            break;
        }
    }

    void dump_unary_operator(std::ofstream& OFS, const UnaryOperator* node) {

        OFS << '{';

        switch (node->get_operator_type())
        {
        case UnaryOperator::UnaryOperatorType::MINUS:
            OFS << '-';
            break;
        case UnaryOperator::UnaryOperatorType::PLUS:
            OFS << '+';
            break;
        
        default:
            throw std::runtime_error("Invalid UnaryOperatorType!");
            break;
        }

        OFS << "|{NEXT}}";
    }

    void dump_binary_operator(std::ofstream& OFS, const BinaryOperator* node) {

        OFS << '{';
        switch (node->get_operator_type())
        {
        case BinaryOperator::BinaryOperatorType::DIVISION:
            OFS << "/";
            break;
        case BinaryOperator::BinaryOperatorType::MINUS:
            OFS << "-";
            break;
        case BinaryOperator::BinaryOperatorType::MULTIPLY:
            OFS << "*";
            break;
        case BinaryOperator::BinaryOperatorType::PLUS:
            OFS << "+";
            break;
        case BinaryOperator::BinaryOperatorType::POWER:
            OFS << "^";
            break;

        default:
            throw std::runtime_error("Invalid BinaryOperatorType!");
            break;
        }

        OFS << "|{LEFT|RIGHT}}";
    }

    void dump_node(std::ofstream& OFS, const FunctionNode* node) {
        OFS << "\t\t\"" << node << "\"" << " [label=\"";

        switch (node->get_type())
        {
        case FunctionNode::NodeType::Number:
            OFS << static_cast<const Number*>(node)->value();
            break;
        case FunctionNode::NodeType::Variable:
            OFS << "x";
            break;
        case FunctionNode::NodeType::UnaryOperator:
            dump_unary_operator(OFS, static_cast<const UnaryOperator*>(node));
            break;
        case FunctionNode::NodeType::BinaryOperator:
            dump_binary_operator(OFS, static_cast<const BinaryOperator*>(node));
            break;

        default:
            throw std::runtime_error("Invalid NodeType!"); 
            break;
        }

        OFS << "\"];\n";
    }
} // namespace

FunctionDumper::FunctionDumper(const std::string& filename, const FunctionNode *root) : OFS_(filename) {
    add_record(nodes_, edges_, root);
}

void FunctionDumper::operator() () {
    #define RECORD(TEXT_) OFS_ << "\t\t" << TEXT_ ;

    OFS_ << "digraph G {\n\t{\n";
    RECORD("rankdir=TB\n");
    RECORD("node [shape=record];\n\n");

    for (const auto& node : nodes_) {
        dump_node(OFS_, node);
    }

    OFS_ << "\n";
    
    for (const auto& edge : edges_) {
        RECORD("\"" << edge.first << "\"" << " -> " << "\"" << edge.second << "\"" << ";\n");
    }

    OFS_ << "\t}\n}";

    #undef RECORD
}