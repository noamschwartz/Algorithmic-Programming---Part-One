

#include "ConnectCommand.h"
#include "ConnectToServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>


/**
 * do the command
 * @param vector the vector of the skript
 * @param index the  index of the current command
 * @return int, number of steps for the next command in the vector
 */
int ConnectCommand::doCommand(const vector<string> &vector, int index) {
    string hostString  = vector.at(index+1);
    char * hostChar = stringToChar(hostString);
    int times = std::stoi (vector.at(index+2));
    //check if two number parameters were given
    if (times == 0){
        throw "Wrong parameters!";
    }
    //create a new connect to server object with the current parms
    //save it in the ds parm
    this->ds = new ConnectToServer(times, hostChar);
    //create a new thread and save him like a member.
    this->test_thread1 = thread(&ConnectToServer::openSocket, this->ds);
    return 3;
}

/**
 * turn the string to char buffer
 * @param s the current string
 * @return the char buffer
 */
char* ConnectCommand::stringToChar(string s){
    char *bufferToReturn = new char [s.length()+1];
    return strcpy(bufferToReturn, s.c_str());

}

/**
  * distructor. distruct the current thread.
 */
ConnectCommand::~ConnectCommand(){
    this->test_thread1.join();
    delete (this->ds);
}
