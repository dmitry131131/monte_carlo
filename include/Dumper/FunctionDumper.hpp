/*!
    \file
    \brief This file describe Function AST dumper
*/

#pragma once

#include <fstream>
#include <vector>
#include "Algorithm/Function.hpp"

class FunctionDumper final {
public:
    using nodes_vector = std::vector<const FunctionNode*>;
    using edges_vector = std::vector<std::pair<const FunctionNode*, const FunctionNode*>>;
private:
    std::ofstream OFS_;
    nodes_vector nodes_;
    edges_vector edges_;

public:
    FunctionDumper(const std::string& filename, const FunctionNode *node);
    void operator() ();
};