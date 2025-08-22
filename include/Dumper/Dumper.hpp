// This file implements dumper class for Algorithm::Result
#pragma once

#include <iostream>
#include <fstream>
#include "Algorithm/Algorithm.hpp"

/// Base dumper class that declare common dumper structure 
class Dumper {
public:
    virtual void dump(const Algorithm::Result &Result) = 0;
    virtual ~Dumper() = default;
};

/// OstreamDumper dumps result into ostream
class OstreamDumper : public Dumper {
protected:
    std::ostream &OS_;
public: 
    explicit OstreamDumper(std::ostream &OS) : OS_(OS) {}
    virtual ~OstreamDumper() = default;

    void dump(const Algorithm::Result &Result) override;
};

/// ColorDumper dumps result into ostream in colored format (Escape codes)
class ColorDumper final : public OstreamDumper {
private:
    enum class StyleCode : char {
        RESET      = 0,
        // Text styles
        BOLD       = 1,
        UNDERLINE  = 4,
        // ANSI escape codes for colors
        BLACK      = 30,
        RED        = 31,
        GREEN      = 32,
        YELLOW     = 33,
        BLUE       = 34,
        MAGENTA    = 35,
        CYAN       = 36,
        WHITE      = 37,
        // Background colors
        BG_BLACK   = 40,
        BG_RED     = 41,
        BG_GREEN   = 42,
        BG_YELLOW  = 43,
        BG_BLUE    = 44,
        BG_MAGENTA = 45,
        BG_CYAN    = 46,
        BG_WHITE   = 47
    };
    // Generate escape code from enum
    std::string get_escape_code(StyleCode code) {return "\033[" + std::to_string(static_cast<char>(code)) + "m";}
    void set_style(StyleCode code) {OS_ << get_escape_code(code);}
    void set_style(const std::initializer_list<StyleCode> &list) {
        for (const auto &code : list) {
            OS_ << get_escape_code(code);
        }
    }

public:
    explicit ColorDumper(std::ostream &OS) : OstreamDumper(OS) {}

    void dump(const Algorithm::Result &Result) override;
};

/// MDDumper dumps result into markdown 
class MDDumper final : public Dumper {
private:
    std::ofstream OS_;
public:
    explicit MDDumper(const std::string &str) : OS_(str) {}
    void dump(const Algorithm::Result &Result) override;
};
