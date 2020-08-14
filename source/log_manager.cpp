//
// log_manager.cpp
// ~~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <fstream>
#include <future>

#include "../headers/nlohmann/json.hpp"
#include "../headers/josephus.hpp"

using json = nlohmann::json;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global variable definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global function definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// josephus::LogManager member definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

josephus::LogManager::LogManager()
{
    this->logLevel = josephus::enums::level::INFO;
    this->hasName = false;
    this->hasTarget = false;
    this->running = false;
    this->currentEntry = nullptr;
    
    auto now = floor<std::chrono::days>(std::chrono::system_clock::now());
    this->currentDay = now.time_since_epoch().count();
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::getInstance
///////////////////////////////////////////////////////////////////////////////

josephus::LogManager *josephus::LogManager::getInstance()
{
    std::lock_guard<std::mutex> instanceLock(josephus::LogManager::instanceMutex);
    if(!josephus::LogManager::hasInstance){
        josephus::LogManager::instance = new josephus::LogManager();
        josephus::LogManager::hasInstance = true;
    }
    
    return(josephus::LogManager::instance);
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::getLogger
///////////////////////////////////////////////////////////////////////////////

josephus::Logger *josephus::LogManager::getLogger(const std::string& tSpace,const std::string& tClass,
                                                  const std::string& tMethod)
{
    josephus::Logger *newLogger{nullptr};
    if(josephus::LogManager::running)
    {
        newLogger = new josephus::Logger(tSpace, tClass, tMethod, josephus::LogManager::instance->logLevel);
    }
    return(newLogger);
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::killInstance
///////////////////////////////////////////////////////////////////////////////

void josephus::LogManager::killInstance()
{
    std::lock_guard<std::mutex> instanceLock(josephus::LogManager::instanceMutex);
    if(josephus::LogManager::hasInstance){
        delete josephus::LogManager::instance;
        josephus::LogManager::instance = nullptr;
        josephus::LogManager::hasInstance = false;
    }

    return;
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::openFile
///////////////////////////////////////////////////////////////////////////////

bool josephus::LogManager::openFile()
{
    bool returnValue{false};
    if(this->hasTarget)
    {
        if(this->outFile.is_open())
        {
            this->outFile.close();
        }

        std::string fileName{this->logTarget};
        if(this->appName.size() > 0)
        {
            fileName.append(this->appName);
            fileName.append("_");
        }
        fileName.append(std::to_string(this->currentDay));
        fileName.append(".log");
        this->outFile.open(fileName, std::ios::in);
        if(this->outFile.is_open())
        {
            this->outFile.close();
            this->outFile.open(fileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
        } else 
        {
            this->outFile.open(fileName, std::ios::out | std::ios::binary | std::ios::ate);
        }
        if(this->outFile.is_open())
        {
            if(this->outFile.tellp() == 0)
            {
                this->outFile.write("[]", 2);
                this->outFile.flush();
            }
            returnValue = true;
        }        
    }    
    return(returnValue);
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::runManager
///////////////////////////////////////////////////////////////////////////////

bool josephus::LogManager::runManager(std::future<void> exitSignal)
{
    josephus::LogManager::running = true;
    auto signalStatus{exitSignal.wait_for(std::chrono::milliseconds(0))};
    while(signalStatus != std::future_status::ready)
    {
        if(this->queue.size() > 0)
        {
            std::lock_guard<std::mutex> queueLock(josephus::LogManager::queueMutex);
            this->currentEntry = josephus::LogManager::queue.front();
            josephus::LogManager::queue.pop();
        }
        if(this->currentEntry != nullptr)
        {
            this->writeEntry();
        }
        signalStatus = exitSignal.wait_for(std::chrono::milliseconds(0));
    }
    josephus::LogManager::running = false;
    while(josephus::Logger::getInstanceCount() > 0){
        continue;
    }
    while(this->queue.size() > 0)
    {
        std::lock_guard<std::mutex> queueLock(josephus::LogManager::queueMutex);
        this->currentEntry = josephus::LogManager::queue.front();
        josephus::LogManager::queue.pop();
        this->writeEntry();
    }
    this->outFile.close();
    return(true);
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::queueEntry
///////////////////////////////////////////////////////////////////////////////

void josephus::LogManager::queueEntry(josephus::LogEntry* newEntry)
{
    std::lock_guard<std::mutex> queueLock(josephus::LogManager::queueMutex);
    josephus::LogManager::queue.push(newEntry);
    return;
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::setAppName
///////////////////////////////////////////////////////////////////////////////

void josephus::LogManager::setAppName(const std::string& newName)
{
    this->appName = newName;
    this->hasName = true;
    return;
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::setLevel
///////////////////////////////////////////////////////////////////////////////

void josephus::LogManager::setLevel(josephus::enums::level newLevel)
{
    this->logLevel = newLevel;
    return;
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::setTargetDirectory
///////////////////////////////////////////////////////////////////////////////

bool josephus::LogManager::setTargetDirectory(const std::string& newTarget)
{
    this->logTarget = newTarget;
    if(this->logTarget[this->logTarget.size() - 1] != '/')
    {
        this->logTarget.append("/");
    }
    this->hasTarget = true;
    return(this->openFile());
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::start
///////////////////////////////////////////////////////////////////////////////

bool josephus::LogManager::start()
{
    bool safeStart{this->hasTarget};
    if(safeStart)
    {
        auto funcPtr{&josephus::LogManager::runManager};
        std::future<void> futureExit{this->killRunManager.get_future()};
        this->catchRunManager = std::async(std::launch::async, funcPtr, this, std::move(futureExit));
        while(!josephus::LogManager::running)
        {
            continue;
        }
    }    
    return(safeStart);
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::stop
///////////////////////////////////////////////////////////////////////////////

bool josephus::LogManager::stop()
{
    bool managerFinished{false};
    this->killRunManager.set_value();
    if(this->catchRunManager.valid())
    {
        managerFinished = this->catchRunManager.get();
        if(!managerFinished)
        {
            throw std::runtime_error("wtf: LogManager::runManager returned false");
        }
    }
    return(managerFinished);
}

///////////////////////////////////////////////////////////////////////////////
// josephus::LogManager::writeEntry
///////////////////////////////////////////////////////////////////////////////

void josephus::LogManager::writeEntry()
{
    size_t newCurrentDay = this->currentEntry->getDay();
    if(newCurrentDay > this->currentDay){
        this->currentDay = newCurrentDay;
        this->openFile();
    }
    this->outFile.seekp(-1, std::ios::cur);
    if(this->outFile.tellp() > 1)
    {
        this->outFile.write(",", 1);
    }
    this->outFile << this->currentEntry->toString();
    this->outFile.write("]", 1);
    this->outFile.flush();
    delete this->currentEntry;
    this->currentEntry = nullptr;
    return;
}
    

///////////////////////////////////////////////////////////////////////////////
// Deconstructor
///////////////////////////////////////////////////////////////////////////////

josephus::LogManager::~LogManager()
{
    
    
}