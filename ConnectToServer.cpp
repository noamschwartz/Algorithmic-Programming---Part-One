#include "ConnectToServer.h"
//CLIENT

using namespace std;
/**
 * constructor.
 * @param portTo the number of port
 * @param hostTo the host
 */
ConnectToServer::ConnectToServer(int portTo, char* hostTo) {
    this->host= hostTo;
    this->port = portTo;
}
/**
 * connect to the servar
 * @param stringToServer the string of the path+number
 */
void ConnectToServer::executeConnectServer(string stringToServer){
    int n = 0;
    char buffer[256];

    n = write(this->sockfd, stringToChar(stringToServer), stringToServer.length());

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    /* Now read server response */
    bzero(buffer,256);
    n = read(this->sockfd, buffer, 256);

    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

}
/**
 * convert string into char buffer.
 * @param s the string to convert.
 * @return buffer of chars.
 */
char* ConnectToServer::stringToChar(string s){
    char *bufferToReturn = new char [s.length()+1];
    return strcpy(bufferToReturn, s.c_str());

}
/**
 * open the socket.
 * a tread witch opened in connectcommand.
 */
void ConnectToServer::openSocket(){
    int portno;
    portno = this->port;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    /* Create a socket point */

    this->sockfd= socket(AF_INET, SOCK_STREAM, 0);

    if (this->sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(this->host);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(this->sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

}
