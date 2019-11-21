#ifndef BASICNETWORKING_H
#define BASICNETWORKING_H

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

/*
 * b64pack compression algorithm
 * #include "b64pack.h"
 * 
 * add the following to cmakelists
 * BasicNetworking/include/b64pack/b64pack.c BasicNetworking/include/b64pack/compress.c BasicNetworking/include/b64pack/io.c BasicNetworking/include/b64pack/error.c
*/

#define BNP_VER 1.0
#define PROT_VER 1
#define BN_PORT 663             //Default Port number
#define MAXDATASIZE 1024        //Max data in bytes
#define BN_AFIP AF_INET         //ipv4(inet) or ipv6(inet6)
#define BN_PROT SOCK_STREAM     //tcp(stream) or udp (dpack)

using namespace std;

class basicNetworking {
    private:
        //Data transfer
        int sendData(string type, string name, string data);
        int recvData(int sock);
        int putData(string data);
        
        //Compresion stuffs
        string compress(string data, int compresionAlg);
        string uncompress(string compressedData, int compresionAlg);
        
        //Encryption stuffs
        int genKey();
        int initHandshake(int sock);
        int recvHandshake(int sock);
        string encrypt(string data, int encryptionAlg);
        string decrypt(string encryptedData, int encryptionAlg);
    public:
        basicNetworking();
        
        //Basic setup
        int setupClient(string ServerAddr);
        int setupServer();
        int usePort(int port);
        void quit();
        int compressData(int compression, bool enable);
        int encryptData(int encryption, bool enable);
        void getInfo();
        
        //Send data
        int sendChar(string name, char data);
        int sendDouble(string name, double num);
        int sendFloat(string name, float num);
        int sendInt(string name, int num);
        int sendStr(string name, string str);
        //int sendOther();
        
        //Recive data
        char getChar(string name);
        double getDouble(string name);
        float getFloat(string name);
        int getInt(string name);
        string getStr(string name);
    
};

#endif