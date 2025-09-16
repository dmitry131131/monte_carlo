/*!
    \file
    \brief This file define error control macro
*/
#pragma once

#include <iostream>
#include <optional>
#include <stdexcept>
		

// TODO Сделать эффективную систему возврата кода(строки) ошибки
using Error_t = std::optional<std::string>;

#define ERROR_MSG(MSG_) do{         \
    std::cerr << MSG_ << std::endl; \
}while(0)

#define MSG(MSG_) do {              \
    std::cout << MSG_ << std::endl; \
}while(0)
