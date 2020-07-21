# josephus
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/a9dca4294cec4f4b82398e4990db939b)](https://www.codacy.com/manual/joseph.adomatis/josephus?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DynasticSponge/josephus&amp;utm_campaign=Badge_Grade)

josephus is a simple C++ logging library
-   Supports multi-threaded applications
-   Supplies JSON formatted file output.
-   Log files auto-rotate daily.

## Stage/Release

josephus is still in an alpha / pre-release stage of development

## Dependencies

josephus makes use of:
-   [nlohmann/json](https://github.com/nlohmann/json/)

## Build

Modify the makefile to address your own folder structure or ensure that "object" and "output" folders exist as peers of the "source" and "headers" folders.

```bash
make static
```
The above will compile the source and generate the libjosephus.a static library in the "output" folder

## Installation

Copy the 'headers' folder and libjosephus.a library file to the appropriate locations within your project's folder structure and update your compiler/linker appropriately.

## Basic Example

```c++
#include <cstdlib>
#include <iostream>
#include <memory>

#include "josephus/josephus_namespace.hpp"
#include "josephus/log_manager.hpp"
#include "josephus/log_enums.hpp"

namespace demoSpace
{
    class demoClass
    {
    public:
        void demoFunction();
    };
}

void demoSpace::demoClass::demoFunction()
{
    std::unique_ptr<josephus::Logger> logger(josephus::LogManager::getLogger("demoSpace", "demoClass", "demoFunction"));
    logger->trace("This is a trace message for demo purposes", "Demo Category");
    logger->debug("This is a debug message for demo purposes", "Demo Category");
    logger->info("This is an info message for demo purposes", "Demo Category");
    logger->warn("This is a warning message for demo purposes", "Demo Category");
    logger->error("This is an error message for demo purposes", "Demo Category");
    logger->fatal("This is a fatal error message for demo purposes", "Demo Category");
    return;
}

int main(int argc, char *argv[])
{
    int returnValue{EXIT_FAILURE};
    josephus::LogManager *logManager{josephus::LogManager::getInstance()};
    logManager->setLevel(josephus::enums::level::INFO);
    logManager->setAppName("josephus_demo");
    if(logManager->setTargetDirectory("/var/tmp/logs/"))
    {
        if(logManager->start())
        {
            demoSpace::demoClass demoObj = demoSpace::demoClass();
            demoObj.demoFunction();
            logManager->stop();
            returnValue = EXIT_SUCCESS;
        }
        else
        {
            std::cerr << "error: could not safely start Log Manager" << std::endl;
        }             
    }
    else
    {
        std::cerr << "error: could not open log file at target" << std::endl;
    } 
    josephus::LogManager::killInstance();
    return(returnValue);
}

/*
Output:
Directory: /var/tmp/logs
File: josephus_demo_18464.log

Content(formatted with whitespace for demo):
[
    {
        "category":"Demo Category",
        "level":"INFO",
        "message":"This is an info message for demo purposes",
        "thread_id":"140229204830016",
        "timestamp":"2020-07-21T04:48:42.632Z",
        "trace":"demoSpace::demoClass::demoFunction"
    },
    {
        "category":"Demo Category",
        "level":"WARNING",
        "message":"This is a warning message for demo purposes",
        "thread_id":"140229204830016",
        "timestamp":"2020-07-21T04:48:42.632Z",
        "trace":"demoSpace::demoClass::demoFunction"
    },
    {
        "category":"Demo Category",
        "level":"ERROR",
        "message":"This is an error message for demo purposes",
        "thread_id":"140229204830016",
        "timestamp":"2020-07-21T04:48:42.632Z",
        "trace":"demoSpace::demoClass::demoFunction"
    },
    {
        "category":"Demo Category",
        "level":"FATAL",
        "message":"This is a fatal error message for demo purposes",
        "thread_id":"140229204830016",
        "timestamp":"2020-07-21T04:48:42.632Z",
        "trace":"demoSpace::demoClass::demoFunction"
    }
]
*/
```

## Contributing

This is a hobby project for me to have fun learning new things and tweaking the results.  If someone is interested, I'm more than happy to share the work/results with whomever wants to make use of it.

If you are interested in contributing however, please open an issue and we can discuss what it is you would like to tackle.  

Thanks!

## License
[MIT](https://choosealicense.com/licenses/mit/)

## Project Namesake(s)
-   Learn about: [Titus Flavius Josephus](https://en.wikipedia.org/wiki/Josephus)