#ifndef PROJECT_DATAREADERSERVER_H
#define PROJECT_DATAREADERSERVER_H


#include "Command.h"
#include "DefineVarCommand.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
/**
 * open a server using a thread that listens to a specific port
 * and reads each line a certain amount of time in a second
 */
class DataReaderServer {
    int times;
    int port;
    map<string, int> pathMap;
    DefineVarCommand *varCommand;
    map<string, pair<string, double>> *mapOfVars;
    bool stop;
    int sockfd;
    int newsockfd;
public:
    /**
     * constructor
     * @param portTo the port
     * @param timesTo time to write
     * @param dfc define var command (for the var command map)
     */
    DataReaderServer( int portTo, int timesTo, DefineVarCommand *dfc);
    /**
     * read from the server
     */
    void openServer();
    void readFromServer();
    /**
     * make the map of paths
     */
    map<string, int> makeMap();
    /**
     * make the vector from the buffer we get from the simulator.
     * @param buffer the buffer.
     * @param bufferSize size of the buffer.
     * @return vector of the buffer.
     */
    vector<string> makeVector(char buffer[], int bufferSize);
    /**
     * get the number from the vector
     * @param path the current path
     * @param buffVec the vector of ths buffer
     * @return the current value
     */
    double getDoubleFromVector(string path,  vector<string> buffVec);
    /**
     * distructor
     */
    ~DataReaderServer();
};


#endif
