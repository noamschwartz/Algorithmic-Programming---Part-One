
#ifndef PROJECT_PRINTCOMMAND_H
#define PROJECT_PRINTCOMMAND_H


#include <map>
#include "Command.h"
#include "DefineVarCommand.h"
#include "EqualsCommand.h"
#include <cmath>
#include <iostream>
#include <ostream>
#include <thread>
#include <thread_db.h>

class PrintCommand : public Command {
    map<string, pair<string, double>> *mapOfVars;
    DefineVarCommand* dfc;
    map<string, Command *> commands;

public:
    PrintCommand(DefineVarCommand *varCommand,  map<string, Command *> commandsMap);
    virtual int doCommand(const vector<string> &vector, int index);
    bool is_number(const std::string &s);
    void printString(string s);
    ~PrintCommand(){};

};



#endif //PROJECT_PRINTCOMMAND_H
