//
// Created by noam on 24/12/18.
//



#include "SleepCommand.h"

using namespace std;
/**
 * constructor
 */
SleepCommand::SleepCommand(){


}
    /**
     * do the command
     * @param vector the vector of the skript
     * @param index the  index of the current command
     * @return int, number of steps for the next command in the vector
     */
int SleepCommand::doCommand(const vector<string> &vector, int index){
    this->toSleep = atof(vector.at(index+1).c_str());
    double time = this->toSleep/1000;
    sleep(time);
    return 2;

}

