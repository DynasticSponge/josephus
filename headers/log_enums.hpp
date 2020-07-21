//
// log_enums.hpp
// ~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_ENUMS_HPP
#define LOG_ENUMS_HPP

#include "josephus_namespace.hpp"

enum class josephus::enums::level 
{
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5
};

#endif