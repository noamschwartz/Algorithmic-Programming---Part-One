

#include "DefineVarCommand.h"

    /**
     * do the command
     * @param vector the vector of the skript
     * @param index the  index of the current command
     * @return return 1 , equals class do this command,so do only one step for the next value in the vector
     */
int DefineVarCommand::doCommand(const vector<string> &vector, int index){
    return 1;
}
    /**
     * @return reference of the map of var
     */
map<string, pair<string, double>> * DefineVarCommand::getMap(){
    return &(this->mapOfVars);
}
