
#ifndef PROJECT_SLEEPCOMMAND_H
#define PROJECT_SLEEPCOMMAND_H


#include "Command.h"
#include <unistd.h>
#include <thread>

class SleepCommand : public Command {
    double toSleep;

public:
    SleepCommand();
    virtual int doCommand(const vector<string> &vector, int index);
    ~SleepCommand(){};

};


#endif //PROJECT_SLEEPCOMMAND_H
