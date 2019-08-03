#ifndef BASICNETWORKING_H
#define BASICNETWORKING_H

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <iostream>

#define PORT 663                //Port number
#define MAXDATASIZE 100         //Max data in bytes
#define BN_AFIP AF_INET         //ipv4(inet) or ipv6(inet6)
#define BN_PROT SOCK_STREAM     //tcp(stream) or udp (dpack)

using namespace std;

class basicNetworking {
    private:
        
    public:
        basicNetworking();
        int setup(string ip);
        int sendChar(char data);
        int sendDouble(double num);
        int sendInt(int num);
    
};

#endif