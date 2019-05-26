#ifndef PROJRCT_OPENSERVERCOMMAND_H
#define PROJRCT_OPENSERVERCOMMAND_H


#include "Command.h"
#include "DefineVarCommand.h"
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include "DataReaderServer.h"
#include <string>

class OpenServerCommand : public Command {
    DefineVarCommand *dfCommand;
    thread test_thread;
    DataReaderServer *ds;
public:
    OpenServerCommand(DefineVarCommand *varCommand);
    virtual ~OpenServerCommand();
    virtual int doCommand(const vector<string> &vector, int index);
};

#endif