#ifndef BASICNETWORKING_H
#define BASICNETWORKING_H

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>

#define BN_PORT 663               //Default Port number
#define MAXDATASIZE 100         //Max data in bytes
#define BN_AFIP AF_INET         //ipv4(inet) or ipv6(inet6)
#define BN_PROT SOCK_STREAM     //tcp(stream) or udp (dpack)

using namespace std;

class basicNetworking {
    private:
        
    public:
        basicNetworking();
        int setupClient(string ServerAddr);
        int setupServer();
        int sendChar(char data);
        int sendDouble(double num);
        int sendFloat(float num);
        int sendInt(int num);
        int sendStr(string str);
        int compressData(int compression);
        int encryptData(int encryption);
    
};

#endif