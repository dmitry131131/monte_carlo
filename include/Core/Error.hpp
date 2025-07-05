// This file define error control macro
#pragma once

#include <iostream>

#define ERROR_MSG(MSG_) do{         \
    std::cerr << MSG_ << std::endl; \
}while(0)

#define MSG(MSG_) do {              \
    std::cout << MSG_ << std::endl; \
}while(0)
