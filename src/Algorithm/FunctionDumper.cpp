#include "Algorithm/FunctionDumper.hpp"

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
} // namespace

FunctionDumper::FunctionDumper(const std::string& filename, const FunctionNode *root) : OFS_(filename) {
    add_record(nodes_, edges_, root);
}


// TODO Add normal dump
void FunctionDumper::operator() () {
    #define RECORD(TEXT_) OFS_ << "\t\t" << TEXT_ ;

    OFS_ << "digraph G {\n\t{\n";
    RECORD("rankdir=TB\n");
    RECORD("node [shape=record];\n\n");

    for (const auto& node : nodes_) {
        RECORD("\"" << node << "\"" << " [label=\"" << node << "\"];\n");
    }

    OFS_ << "\n";
    
    for (const auto& edge : edges_) {
        RECORD("\"" << edge.first << "\"" << " -> " << "\"" << edge.second << "\"" << ";\n");
    }

    OFS_ << "\t}\n}";

    #undef RECORD
}