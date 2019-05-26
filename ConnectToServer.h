#ifndef PROJECT_CONNECTTOSERVER_H
#define PROJECT_CONNECTTOSERVER_H


#include "Command.h"
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
class ConnectToServer {
    int port;
    char *host;
    int sockfd;
public:
    /**
     * constructor.
     * @param portTo the number of port
     * @param hostTo the host
     */
    ConnectToServer(int portTo, char * hostTo);
    /**
     * connect to the servar
     * @param stringToServer the string of the path+number
     */
    void executeConnectServer(string stringToServer);
    /**
     * convert string into char buffer.
     * @param s the string to convert.
     * @return buffer of chars.
     */
    char* stringToChar(string s);
    /**
     * open the socket.
     */
    void openSocket();
    /**
     * distructor.
     */
    ~ConnectToServer(){};

};


#endif //EVEN1_CONNECTTOSERVER_H