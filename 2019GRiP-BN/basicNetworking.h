#ifndef BASICNETWORKING_H
#define BASICNETWORKING_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "663"
#define MAXDATASIZE 100

using namespace std;

class bn {
    private:
       void getInAddr(struct sockaddr *sa);
    public:
        bn();
        int setup(string ip);
        int sendDouble(double foo);
        int sendInt(int foo);
    
};

#endif