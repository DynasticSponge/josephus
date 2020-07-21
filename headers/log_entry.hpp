//
// log_entry.hpp
// ~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_ENTRY_HPP
#define LOG_ENTRY_HPP

#include <chrono>
#include <string>

#include "josephus_namespace.hpp"
#include "log_enums.hpp"
#include "nlohmann/json.hpp"

class josephus::LogEntry
{
public:
    LogEntry();
    size_t getDay();
    void setCategory(const std::string&);
    void setClass(const std::string&);
    void setLevel(josephus::enums::level);
    void setMessage(const std::string&);
    void setMethod(const std::string&);
    void setNamespace(const std::string&);
    void setThread(const std::string&);
    std::string toString();
    ~LogEntry();
protected:
private:
    ////////////////////////////////////////////////////////////////////////////////
    // Private Methods
    ////////////////////////////////////////////////////////////////////////////////
    std::string getISOTimestamp();
    std::string getTracePath();

    ////////////////////////////////////////////////////////////////////////////////
    // Private Properties
    ////////////////////////////////////////////////////////////////////////////////
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    std::string level;
    std::string category;
    std::string message;    
    std::string traceClass;
    std::string traceMethod;
    std::string traceNamespace;
    std::string thread;
    size_t entryDay;    
};

#endif