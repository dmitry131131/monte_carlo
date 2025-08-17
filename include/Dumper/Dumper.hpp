// This file implements dumper class for Algorithm::Result
#pragma once

#include <iostream>
#include "Algorithm/Algorithm.hpp"

/// Base dumper class that declare common dumper structure 
class Dumper {
public:
    virtual void dump(const Algorithm::Result &Result) = 0;
    virtual ~Dumper() = default;
};

/// OstreamDumper dumps result into ostream
class OstreamDumper : public Dumper {
private:
    std::ostream &OS_;
public: 
    OstreamDumper(std::ostream &OS) : OS_(OS) {}
    virtual ~OstreamDumper() = default;

    void dump(const Algorithm::Result &Result) override;
};

// TODO create HTML or MarkDown dumper