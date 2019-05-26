#ifndef PROJECT_IFCOMMAND_H
#define PROJECT_IFCOMMAND_H

#include <map>
#include "Command.h"
#include "DefineVarCommand.h"
#include "EqualsCommand.h"
#include "PrintCommand.h"
#include "SleepCommand.h"
#include <utility>
#include <cmath>

class IfCommand : public Command {
    map<string, pair<string, double>> *mapOfVars;
    map<string, Command*> commandsMap;
    DefineVarCommand* dfc;
public:
    IfCommand(DefineVarCommand* defineVarCommand, map<string, Command*> commands);
    int doCommand(const vector<string> &vector, int index) override;
    void makeCommandsVector(const vector<string> &v, vector<string> &commands, int & curIndex);
    bool is_number(const std::string &s);
    bool checkCondition (double a, double b, string op);
    void doIfCommand(const vector<string> &commands, vector<string> &lineCommands);
    void executeLoop(vector<string> &lineCommands);
    bool isOperator (string op);
    bool isExp (string exp);
    ~IfCommand() override;
};


#endif //PROJECT_IFCOMMAND_H
