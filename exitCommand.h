#ifndef PROJECT_EXITCOMMAND_H
#define PROJECT_EXITCOMMAND_H

#include "Command.h"
#include "SleepCommand.h"
#include "PrintCommand.h"
#include "IfCommand.h"
#include "DefineVarCommand.h"
#include "OpenServerCommand.h"
#include "ConnectCommand.h"
#include "LoopCommand.h"
#include "EqualsCommand.h"
class exitCommand : public Command{
    map<string, Command *> *mapCommands;
public:
    explicit exitCommand(map <string,Command*> *mapOfCommands);
/**
     * do the command
     * @param vector the vector of the skript
     * @param index the  index of the current command
     * @return int, number of steps for the next command in the vector
     */
     int doCommand(const vector<string> &vector, int index) override;
    /**
     * Distructor
     */
    ~exitCommand() override;
};


#endif //PROJECT_EXITCOMMAND_H
