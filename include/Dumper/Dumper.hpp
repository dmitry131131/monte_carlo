/*!
    \file
    \brief This file implements dumper class for Algorithm::Result
*/
#pragma once

#include <iostream>
#include <fstream>
#include "Algorithm/Algorithm.hpp"

/*!
    \brief Interface class with Dumper interface
*/
class Dumper {
public:
    /// Dumper function
    virtual void dump(const Algorithm::Result &Result) = 0;
    virtual ~Dumper() = default;
};

/*!
	\brief Base dumper class of ostream-like dumpers
*/
class OstreamLikeDumper : public Dumper {
protected:
    std::ofstream OFS_;
    std::ostream &OS_ = OFS_;

public:
    OstreamLikeDumper(std::ostream &OS) : OS_(OS) {}
    OstreamLikeDumper(const std::string &str) : OFS_(str) {}

    virtual ~OstreamLikeDumper() = default;
};

/*!
	\brief OstreamDumper dumps result into ostream
*/
class DefaultDumper final : public OstreamLikeDumper {
using OstreamLikeDumper::OstreamLikeDumper;

public: 
    virtual ~DefaultDumper() = default;

    void dump(const Algorithm::Result &Result) override;
};

/*!
	\brief ColorDumper dumps result into ostream in colored format (Escape codes)
*/
class ColorDumper final : public OstreamLikeDumper {
using OstreamLikeDumper::OstreamLikeDumper;
private:
    /// Style codes for creating escape codes  
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
    /// Generate escape code from enum
    std::string get_escape_code(StyleCode code) {return "\033[" + std::to_string(static_cast<char>(code)) + "m";}
    void set_style(StyleCode code) {OS_ << get_escape_code(code);}
    void set_style(const std::initializer_list<StyleCode> &list) {
        for (const auto &code : list) {
            OS_ << get_escape_code(code);
        }
    }

public:
    void dump(const Algorithm::Result &Result) override;
};

/*!
	\brief MDDumper dumps result into markdown 
*/
class MDDumper final : public OstreamLikeDumper {
using OstreamLikeDumper::OstreamLikeDumper;
public:
    void dump(const Algorithm::Result &Result) override;
};
