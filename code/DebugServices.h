#ifndef DEBUGSERVICES_H
#define DEBUGSERVICES_H

#include <ctime>
#include <cstring>
#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <iomanip>
#include <limits>
#include "imgui.h"
#include "imgui-SFML.h"

// Macro: DBG_SERVICES
// Shortcut for DebugServices::get()
#define DBG_SERVICES DebugServices::get()

// Macro: DBG_CONSOLE
#define DBG_CONSOLE DebugServices::get().outputToConsole(str)
#define DBG_CONSOLE_LINE DebugServices::get().outputToConsole(str + "\n")

// Macro: TIMED_BLOCK
// Call it at the beginning of a block to keep track of how much time is spent to execute it.
// Get the individual result(s) using DBG_SERVICES.getTimedBlockRecords(DebugRecordFilter)
// Get the overtime info result using DBG_SERVICES.getTimedBlockOvertimeInfo(std;:string fileName, std::string functionName)
#define TIMED_BLOCK _TimedBlock timedBlock_##__LINE__(DBG_SERVICES.getFileName(__FILE__), __LINE__, __FUNCTION__)

// Macro: WATCH_VARIABLE(v)
// Pass any variable v of time T that implements operator << (std::ostream& os, T t)
// Get the result(s) using DBG_SERVICES.getWatchedVariableRecords(DebugRecordFilter)
#define WATCH_VARIABLE(variable) DBG_SERVICES._watchVariable(variable, #variable, DBG_SERVICES.getFileName(__FILE__), __LINE__, __FUNCTION__, std::numeric_limits<int>::max())
#define WATCH_VARIABLE_ONCE(variable) DBG_SERVICES._watchVariable(variable, #variable, DBG_SERVICES.getFileName(__FILE__), __LINE__, __FUNCTION__, 1)
#define WATCH_VARIABLE_COUNT(variable, count) DBG_SERVICES._watchVariable(variable, #variable, DBG_SERVICES.getFileName(__FILE__), __LINE__, __FUNCTION__, count)

// Macro: ERROR_MESSAGE
// Pass a message.
#define ERROR_MESSAGE(message) DBG_SERVICES.pushMessage(message, "Error", DBG_SERVICES.getFileName(__FILE__), __LINE__, __FUNCTION__)

// Macro: WARNING_MESSAGE
// Pass a message.
// Get the result(s) using DBG_SERVICES.getErrorMessageRecords(DebugRecordFilter)
#define WARNING_MESSAGE(message) DBG_SERVICES.pushMessage(message, "Warning", DBG_SERVICES.getFileName(__FILE__), __LINE__, __FUNCTION__)

// Macro: DEBUG_MESSAGE
// Pass a message.
// Get the result(s) using DBG_SERVICES.getDebugMessageRecords(DebugRecordFilter)
#define DEBUG_MESSAGE(message) DBG_SERVICES.pushMessage(message, "Message", DBG_SERVICES.getFileName(__FILE__), __LINE__, __FUNCTION__)


struct MessageRecord {
    std::string fileName;
    int lineNumber;
    std::string functionName;
    std::string fullAddress;
    double timeStamp;

    std::string type;
    std::string message;
};

struct WatchedVariableRecord {
    std::string fileName;
    int lineNumber;
    std::string functionName;
    std::string fullAddress;
    std::string variableAddress;
    double timeStamp;

    std::string name;
    std::string value;
};

struct TimedBlockRecord {
    std::string fileName;
    int lineNumber;
    std::string functionName;
    std::string fullID;
    unsigned int debugFrame;

    double timeSpent;
    double startTimeStamp;
    double stopTimeStamp;
};

struct TimedBlockOvertimeInfo {
    TimedBlockOvertimeInfo() {
        sum = 0;
        records = 0;
    }
    std::string fileName;
    int lineNumber;
    std::string functionName;
    std::string fullID;

    double average;
    double min;
    double max;
    double sum;
    int records;
};

struct DebugRecordFilters {
    DebugRecordFilters() {
        matchRecordEntry = -1;
        minRecordEntry = -1;
        maxRecordEntry = std::numeric_limits<int>::max();
        minTimeStamp = -1;
        maxTimeStamp = std::numeric_limits<double>::max();
    }
    std::string fromFile;
    std::string fromFunction;
    int matchRecordEntry;
    int minRecordEntry;
    int maxRecordEntry;
    double minTimeStamp;
    double maxTimeStamp;
};

// Auxiliary structure for TIMED_BLOCK macro, do not use.
struct _TimedBlock {
    _TimedBlock(std::string fileName, int lineNumber, std::string functionName);
    ~_TimedBlock();

    std::string fileName;
    int lineNumber;
    std::string functionName;

    std::clock_t start;
};

class DebugServices {
    public:
        static DebugServices& get();

        void initialize(sf::RenderWindow* window);
        void showUI();
        void pause();
        void unpause();

        void pushMessage(std::string message, std::string type, std::string fileName, int lineNumber, std::string functionName);
        void outputToConsole(std::string message);

        void enableFilePersistentTimedBlockOutput(std::string fileName, std::ostream* outStream = nullptr);
        void enableFunctionPersistentTimedBlockOutput(std::string fileName, std::string functionName, std::ostream* outStream = nullptr);

        void enableFunctionPersistentWatchedVaribleOutput(std::string fileName, std::string functionName, std::ostream* outStream = nullptr);

        std::string getFileName(std::string path);

        // Do not use these, use macro TIMED_BLOCK
        void _addTimedBlockRecord(_TimedBlock& timedBlock);

        // Do not use this, use macro WATCH_VARIABLE
        template <typename T>
        void _watchVariable(T value, std::string variableName, std::string fileName, int lineNumber, std::string functionName, int count = std::numeric_limits<int>::max()) {
            std::string fullAddress = getFormatedAdress(fileName, lineNumber, functionName);
            std::string varAddress = fullAddress + " -- " + variableName;
            if (watchedVariableRecords[varAddress].size() >= count) return;
            WatchedVariableRecord record;
            record.timeStamp = diffclock(clock(), clockDebugStart);
            record.fileName = fileName;
            record.lineNumber = lineNumber;
            record.functionName = functionName;
            record.fullAddress = fullAddress;
            record.variableAddress = varAddress;
            std::stringstream ss;
            ss << std::setprecision(6) << value;
            record.value = ss.str();
            record.name = variableName;
            //record.recordEntry = mapWatchedVariableRecords[fileName][functionName].size() + 1;
            watchedVariableRecords[record.variableAddress].push_back(record);
            if (watchedVariableRecords[varAddress].size() >= maxStoredWatchedVariableEntries) {
                watchedVariableRecords[record.variableAddress].pop_front();
            };

            /*
            std::map<std::string, std::map<std::string, std::ostream*> >::iterator it =
                                                        mapFunctionPersistentWatchedVariableOutput.find(fileName);

            if (it != mapFunctionPersistentWatchedVariableOutput.end()){
                std::map<std::string, std::ostream*>::iterator it2 = it->second.find(functionName);
                if (it2 != it->second.end()){
                    std::ostream& outputStream = *it2->second;
                    outputStream << std::setw(6) << record.recordEntry << " "
                                 << std::setw(6) << std::setprecision(2) << record.timeStamp << "s "
                                 << record.fileName << " "
                                 << record.functionName
                                 << ": " << record.name << " = "
                                 << record.value
                                 << std::endl;
                }
            }
            */
        }

    private:
        DebugServices();
        static DebugServices* instance;

        double diffclock(clock_t c1, clock_t c2);

        std::string getFormatedAdress(std::string fileName, int lineNumber, std::string functionName);

        std::map<std::string, std::list<MessageRecord> > messagesLists;

        std::map<std::string, std::list<TimedBlockRecord> > timedBlockRecords;
        std::map<std::string, TimedBlockOvertimeInfo> timedBlocksOvertimeInfo;
        std::map<std::string, std::list<WatchedVariableRecord> > watchedVariableRecords;

        std::map<std::string, std::map<std::string, std::list<TimedBlockRecord> > > mapTimedBlockRecords;
        std::map<std::string, std::map<std::string, TimedBlockOvertimeInfo> > mapTimedBlockOvertimeInfo;
        std::map<std::string, std::map<std::string, std::list<WatchedVariableRecord> > > mapWatchedVariableRecords;

        const std::clock_t clockDebugStart;
        const unsigned int maxStoredTimedBlockEntries;
        const unsigned int maxStoredWatchedVariableEntries;
        const unsigned int maxStoredMessages;
        unsigned int maxShowedWatchedVariableEntries;
        unsigned int maxShowedTimedBlockEntries;
        unsigned int maxShowedMessages;
        bool isPaused;
        double elapsedTime;
        //filters
        float instant;
        float beginInterval;
        float endInterval;
        char functionFilter[1000];


        std::map<std::string, std::ostream*> mapFilePersistentTimedBlockOutput;
        std::map<std::string, std::map<std::string, std::ostream*> > mapFunctionPersistentTimedBlockOutput;
        std::map<std::string, std::map<std::string, std::ostream*> > mapFunctionPersistentWatchedVariableOutput;

        std::ostream outputStream;
        std::ostream errorOutputStream;
        std::ostringstream ossConsole;
        char commandLine[1000];

        sf::RenderWindow* window;
};


#endif // DEBUGSERVICES_H
