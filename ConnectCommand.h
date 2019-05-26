#ifndef PROJECT_CONNECTCOMMAND_H
#define PROJECT_CONNECTCOMMAND_H

#include "Command.h"
#include "ConnectToServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

class ConnectCommand : public Command {
public:
    ConnectToServer *ds;
    thread test_thread1;
public:
    /**
     * constructor.
     */
    ConnectCommand(){};

    /**
     * do the command
     * @param vector the vector of the skript
     * @param index the  index of the current command
     * @return int, number of steps for the next command in the vector
     */
    int doCommand(const vector<string> &vector, int index) override;

    /**
     * turn the string to char buffer
     * @param s the current string
     * @return the char buffer
     */
    char* stringToChar(string s);
    /**
     * distructor.
     */
    ~ConnectCommand() override;

};

#endif