#include "exitCommand.h"
/**
 * constructor
 * @param mapOfCommands ref to map of commands
 */
exitCommand::exitCommand(map <string,Command*> *mapOfCommands) {
this->mapCommands = mapOfCommands;
}
/**
 * delete all objects in the map
 * @param vector vector of the script
 * @param index the last index in the vector
 * @return 0
 */
int exitCommand::doCommand(const vector<string> &vector, int index) {
    //delete all the objects in the map of commands
    delete(this->mapCommands->at("openDataServer"));
    delete(this->mapCommands->at("connect"));
    delete (this->mapCommands->at("var"));
    delete (this->mapCommands->at("="));
    delete(this->mapCommands->at("while"));
    delete(this->mapCommands->at("if"));
    delete (this->mapCommands->at("print"));
    delete (this->mapCommands->at("sleep"));
    delete (this);
    return 0;
}
/**
 * distructor
 */
exitCommand::~exitCommand() {
delete (this->mapCommands);
}
