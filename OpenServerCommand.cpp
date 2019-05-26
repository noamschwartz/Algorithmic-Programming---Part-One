
#include "OpenServerCommand.h"

/**
 * open the server
 * @param varCommand ref to the var command for the map op commands
 */
OpenServerCommand::OpenServerCommand(DefineVarCommand *varCommand) {
    this->dfCommand = varCommand;
}
/**
 * do the command of the open server
 * @param vector vector of the script
 * @param index the current index of the command
 * @return number of steps for the next command in the vector
 */
int OpenServerCommand::doCommand(const vector<string> &vector, int index) {
    string timesString = (vector.at(index+2));
    string stringPort = vector.at(index+1);

    int port = std::stoi(stringPort);
    int times = std:: stoi(timesString);
    //check if two number parameters were given
    if (port == 0){
        throw "Wrong parameters!";
    }
    //call data reader server
    this->ds = new DataReaderServer(port, times, this->dfCommand);
    this->ds->openServer();
    cout<<"please enter a key"<<endl;
    char c;
    scanf("%c",&c);
    //create a new thread
    this->test_thread = thread(&DataReaderServer::readFromServer, this->ds);

    return 3;
}

/**
 * distructor
 */
OpenServerCommand::~OpenServerCommand(){
    this->test_thread.join();
    delete(this->ds);
}