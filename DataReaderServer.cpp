//
// Created by noam on 19/12/18.
//

#include "DataReaderServer.h"

//OPEN SERVER COMMMAND
//SERVER

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>

#include <string.h>

#include <sys/socket.h>

DataReaderServer::DataReaderServer( int portTo, int timesTo, DefineVarCommand *dfc) {
    this->port = portTo;
    this->times = timesTo;
    this->pathMap = makeMap();
    this->varCommand = dfc;
    this->mapOfVars = this->varCommand->getMap();
    this->stop = false;
    this->newsockfd = 0;
}


void DataReaderServer::openServer(){
    int n=0, newsockfd=0, clilen=0;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];

    /* First call to socket() function */
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->port);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    this->newsockfd = accept(this->sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }
    while (n == 0) {
        n = read(this->newsockfd,buffer,255);
    }

}

void DataReaderServer::readFromServer(){
    int n = 0;
    char buffer[256];
    while(!this->stop) {
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);

        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
        //make a vector out of the buffer
        vector<string> bufferVec = makeVector(buffer, n);
        //iterate over map of vars
        map<string, pair<string, double>>::iterator it;
        for (it = this->mapOfVars->begin(); it != this->mapOfVars->end(); it++) {
            if (it->second.first != " ") {
                double d = getDoubleFromVector(it->second.first, bufferVec);
                it->second.second = d;
                printf("output %f\n", it->second.second);
            }

        }
    }
}


double DataReaderServer::getDoubleFromVector(string path,  vector<string> buffVec){
    string updatedPath = path;
    map<string, int>::iterator pathIt;
    string newString = path.substr(1, path.size()-2);
    pathIt = this->pathMap.find(newString);
    if (pathIt != pathMap.end()) {
        int index = pathIt->second;

        //get number from vector and convert to double
        double numToAssign = atof(buffVec.at(index).c_str());
        return numToAssign;
    }
    return 0;
}
map<string, int> DataReaderServer::makeMap(){
    map<string, int> pathsMap;
    pathsMap["/instrumentation/airspeed-indicator/indicated-speed-kt"] =  0;
    pathsMap["/instrumentation/altimeter/indicated-altitude-ft"]= 1;
    pathsMap["/instrumentation/altimeter/pressure-alt-ft"] =  2;
    pathsMap["/instrumentation/attitude-indicator/indicated-pitch-deg"]= 3;
    pathsMap["/instrumentation/attitude-indicator/indicated-roll-deg"] = 4;
    pathsMap["/instrumentation/attitude-indicator/internal-pitch-deg"] =5;
    pathsMap["/instrumentation/attitude-indicator/internal-roll-deg"] = 6;
    pathsMap["/instrumentation/encoder/indicated-altitude-ft"] =7;
    pathsMap["/instrumentation/encoder/pressure-alt-ft"] =8;
    pathsMap["/instrumentation/gps/indicated-altitude-ft"] = 9;
    pathsMap["/instrumentation/gps/indicated-ground-speed-kt"] =  10;
    pathsMap["/instrumentation/gps/indicated-vertical-speed"] = 11;
    pathsMap["/instrumentation/heading-indicator/indicated-heading-deg"] =  12;
    pathsMap["/instrumentation/magnetic-compass/indicated-heading-deg"] =  13;
    pathsMap["/instrumentation/slip-skid-ball/indicated-slip-skid"] = 14;
    pathsMap["/instrumentation/turn-indicator/indicated-turn-rate"] = 15;
    pathsMap["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"] = 16;
    pathsMap["/controls/flight/aileron"] = 17;
    pathsMap["/controls/flight/elevator"] = 18;
    pathsMap["/controls/flight/rudder"] = 19;
    pathsMap["/controls/flight/flaps"] = 20;
    pathsMap["/controls/engines/current-/engine/throttle"] = 21;
    pathsMap["/engines/engine/rpm"] =22;
    return pathsMap;

}
//make a vector from the buffer. each value will be in its own compartment.
vector<string> DataReaderServer::makeVector(char buffer[], int bufferSize) {
    vector<string> vecToReturn;
    string temp;
    for (int j = 0; j < bufferSize; j++) {
        if (buffer[j] == '\n') {
            buffer[j] = ',';

        }
    }
    int i = 0;
    //check if to change and use n as the size of the buffer
    while (i < bufferSize) {
        if (buffer[i] != ',') {
            temp = temp + buffer[i];
            i++;
        } else {
            //temp = getRidOfEndl(temp);
            vecToReturn.push_back(temp);
            temp = "";
            i++;
        }
    }
    //temp = getRidOfEndl(temp);
    vecToReturn.push_back(temp);
    return vecToReturn;
}

DataReaderServer::~DataReaderServer() {
    //close the sockets
    close(this->sockfd);
    close(this->newsockfd);
    //stop the loop
    this->stop = true;
}