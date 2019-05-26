#ifndef PROJECT_COMMAND_H
#define PROJECT_COMMAND_H
#include <iostream>
#include <vector>
#include <thread>
using namespace std;
/**
 * an abstruct class , is an interface for all commands
 */
class Command {
public:
    /**
     * do the command
     * @param vector the vector of the skript
     * @param index the  index of the current command
     * @return int, number of steps for the next command in the vector
     */
    virtual int doCommand(const vector<string> &vector, int index) = 0;
    /**
     * Distructor
     */
    virtual ~Command(){};
};




#endif