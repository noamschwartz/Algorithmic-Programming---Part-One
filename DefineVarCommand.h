#ifndef PROJECT_DEFINEVARCOMMAND_H
#define PROJECT_DEFINEVARCOMMAND_H

#include "Command.h"
#include <map>

class DefineVarCommand : public Command {
    map<string, pair<string, double>> mapOfVars;
public:
    /**
     * constructor.
     */
    DefineVarCommand(){};
    /**
     * do the command
     * @param vector the vector of the skript
     * @param index the  index of the current command
     * @return int, number of steps for the next command in the vector
     */
    virtual int doCommand(const vector<string> &vector, int index);
    /**
     * @return reference of the map of var
     */
    map<string, pair<string, double>> * getMap();
    ~DefineVarCommand() override {};

};

#endif