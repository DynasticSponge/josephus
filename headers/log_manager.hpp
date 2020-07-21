//
// log_manager.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

#include <fstream>
#include <future>
#include <mutex>
#include <queue>
#include <string>

#include "josephus_namespace.hpp"
#include "log_enums.hpp"
#include "log_logger.hpp"

class josephus::LogManager 
{
public:
    static josephus::LogManager *getInstance();
    static josephus::Logger *getLogger(const std::string&,const std::string&,const std::string&);
    static void killInstance();
    static void queueEntry(josephus::LogEntry*);    
    void setLevel(josephus::enums::level);
    void setAppName(const std::string&);
    bool setTargetDirectory(const std::string&);
    bool start();
    bool stop();

protected:
private:
    ////////////////////////////////////////////////////////////////////////////////
    // Private Methods
    ////////////////////////////////////////////////////////////////////////////////
    LogManager();
    bool openFile();
    bool runManager(std::future<void>);
    void writeEntry();
    ~LogManager();
    
    ////////////////////////////////////////////////////////////////////////////////
    // Private Properties
    ////////////////////////////////////////////////////////////////////////////////
    static inline josephus::LogManager *instance = nullptr;
    static inline bool hasInstance = false;
    static inline bool running = false;
    static inline std::mutex instanceMutex = std::mutex();
    static inline std::mutex queueMutex = std::mutex();
    static inline std::queue<josephus::LogEntry*> queue = std::queue<josephus::LogEntry*>();
    
    std::future<bool> catchRunManager;
    std::promise<void> killRunManager;
    std::fstream outFile;
    josephus::LogEntry *currentEntry;
    josephus::enums::level logLevel;
    std::string appName;
    std::string logTarget;
    bool hasName;
    bool hasTarget;
    size_t currentDay;
          
};

#endif