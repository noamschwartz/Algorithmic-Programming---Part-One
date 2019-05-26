#ifndef PROJECT_LOOPCOMMAND_H
#define PROJECT_LOOPCOMMAND_H

#include "Command.h"
#include <vector>
#include <iostream>
#include "DefineVarCommand.h"
#include "EqualsCommand.h"
#include "PrintCommand.h"
#include "IfCommand.h"
#include "SleepCommand.h"
#include <cmath>
#include <utility>
using namespace std;

class LoopCommand : public Command{
    map<string, pair<string, double>> *mapOfVars;
    map<string, Command*> commandsMap;
    DefineVarCommand* dfc;
public:
    LoopCommand(DefineVarCommand* defineVarCommand, map<string, Command*> commands);
    int doCommand(const vector<string> &vector, int index) override;
    void makeCommandsVector(const vector<string> &v, vector<string> &commands, int & curIndex);
    bool is_number(const std::string &s);
    void executeLoop(vector<string> &lineCommands);
    void doWhileCommand(const vector<string> &commands, vector<string> &lineCommands);
    void recognizeLoopOp(const vector<string> &commands, vector<string> &lineCommands,string op, const vector<string> &before, const vector<string> &after);
    bool isOperator (string op);
    bool isExp (string exp);
    double evaluateTheString (const vector<string> &s, EqualsCommand* e, vector<string> &temp);
    ~LoopCommand() override {};
};


#endif //PROJECT_LOOPCOMMAND_H
