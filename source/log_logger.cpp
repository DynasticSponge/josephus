//
// log_logger.cpp
// ~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <sstream>
#include <thread>

#include "../headers/josephus_namespace.hpp"
#include "../headers/log_entry.hpp"
#include "../headers/log_enums.hpp"
#include "../headers/log_logger.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global variable definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global function definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// josephus::Logger member definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

josephus::Logger::Logger(const std::string& tSpace,const std::string& tClass,
                         const std::string& tMethod, josephus::enums::level level)
{
    josephus::Logger::instanceCount++;
    this->traceNamespace = tSpace;
    this->traceClass = tClass;
    this->traceMethod = tMethod;
    this->logLevel = level;
    std::stringstream ostream;
    ostream << std::this_thread::get_id();
    ostream >> this->threadID;    
}

///////////////////////////////////////////////////////////////////////////////
// josephus::Logger::debug
///////////////////////////////////////////////////////////////////////////////

void josephus::Logger::debug(const std::string& lMsg, const std::string& lCat)
{
    if(this->logLevel <= josephus::enums::level::DEBUG)
    {
        josephus::LogEntry *newEntry{this->initEntry(lMsg, lCat)};
        newEntry->setLevel(josephus::enums::level::DEBUG);
        josephus::LogManager::queueEntry(newEntry);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
// josephus::Logger::error
///////////////////////////////////////////////////////////////////////////////

void josephus::Logger::error(const std::string& lMsg, const std::string& lCat)
{
    if(this->logLevel <= josephus::enums::level::ERROR)
    {
        josephus::LogEntry *newEntry{this->initEntry(lMsg, lCat)};
        newEntry->setLevel(josephus::enums::level::ERROR);
        josephus::LogManager::queueEntry(newEntry);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
// josephus::Logger::fatal
///////////////////////////////////////////////////////////////////////////////

void josephus::Logger::fatal(const std::string& lMsg, const std::string& lCat)
{
    if(this->logLevel <= josephus::enums::level::FATAL)
    {
        josephus::LogEntry *newEntry{this->initEntry(lMsg, lCat)};
        newEntry->setLevel(josephus::enums::level::FATAL);
        josephus::LogManager::queueEntry(newEntry);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
// josephus::Logger::getInstanceCount()
///////////////////////////////////////////////////////////////////////////////

size_t josephus::Logger::getInstanceCount()
{
    return(josephus::Logger::instanceCount);
}

///////////////////////////////////////////////////////////////////////////////
// josephus::Logger::info
///////////////////////////////////////////////////////////////////////////////

void josephus::Logger::info(const std::string& lMsg, const std::string& lCat)
{
    if(this->logLevel <= josephus::enums::level::INFO)
    {
        josephus::LogEntry *newEntry{this->initEntry(lMsg, lCat)};
        newEntry->setLevel(josephus::enums::level::INFO);
        josephus::LogManager::queueEntry(newEntry);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
// josephus::Logger::initEntry
///////////////////////////////////////////////////////////////////////////////

josephus::LogEntry *josephus::Logger::initEntry(const std::string& lMsg, const std::string& lCat)
{
    josephus::LogEntry *entry = new josephus::LogEntry();
    entry->setCategory(lCat);
    entry->setClass(this->traceClass);
    entry->setMessage(lMsg);
    entry->setMethod(this->traceMethod);
    entry->setNamespace(this->traceNamespace);
    entry->setThread(this->threadID);
    return(entry);
}

///////////////////////////////////////////////////////////////////////////////
// josephus::Logger::trace
///////////////////////////////////////////////////////////////////////////////

void josephus::Logger::trace(const std::string& lMsg, const std::string& lCat)
{
    if(this->logLevel <= josephus::enums::level::TRACE)
    {
        josephus::LogEntry *newEntry{this->initEntry(lMsg, lCat)};
        newEntry->setLevel(josephus::enums::level::TRACE);
        josephus::LogManager::queueEntry(newEntry);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
// josephus::Logger::warn
///////////////////////////////////////////////////////////////////////////////

void josephus::Logger::warn(const std::string& lMsg, const std::string& lCat)
{
    if(this->logLevel <= josephus::enums::level::WARN)
    {
        josephus::LogEntry *newEntry{this->initEntry(lMsg, lCat)};
        newEntry->setLevel(josephus::enums::level::WARN);
        josephus::LogManager::queueEntry(newEntry);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
// Deconstructor
///////////////////////////////////////////////////////////////////////////////

josephus::Logger::~Logger()
{
    josephus::Logger::instanceCount--;
}