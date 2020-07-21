//
// log_logger.hpp
// ~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_LOGGER_HPP
#define LOG_LOGGER_HPP

#include "josephus_namespace.hpp"
#include "log_enums.hpp"
#include "log_manager.hpp"

class josephus::Logger 
{
public:
    static size_t getInstanceCount();
    void debug(const std::string&, const std::string& = "");
    void error(const std::string&, const std::string& = "");
    void fatal(const std::string&, const std::string& = "");
    void info(const std::string&, const std::string& = "");
    void trace(const std::string&, const std::string& = "");
    void warn(const std::string&, const std::string& = "");
    ~Logger();
protected:
private:
    ////////////////////////////////////////////////////////////////////////////////
    // Private Methods
    ////////////////////////////////////////////////////////////////////////////////
    friend class josephus::LogManager;
    explicit Logger(const std::string&, const std::string&, const std::string&, josephus::enums::level);
    josephus::LogEntry *initEntry(const std::string&, const std::string&);
    

    ////////////////////////////////////////////////////////////////////////////////
    // Private Properties
    ////////////////////////////////////////////////////////////////////////////////
    static inline size_t instanceCount = 0; 
    josephus::enums::level logLevel;
    std::string traceNamespace;
    std::string traceClass;
    std::string traceMethod;
    std::string threadID;
};

#endif