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

#include "sqlite3.h"

#define BNP_VER 1.0
#define PROT_VER 1.0
#define BN_PORT 663             //Default Port number
#define MAXDATASIZE 1024        //Max data in bytes
#define BN_AFIP AF_INET         //ipv4(inet) or ipv6(inet6)
#define BN_PROT SOCK_STREAM     //tcp(stream) or udp (dpack)

#define DB_NAME "database.sqlite"

using namespace std;

class basicNetworking {
    private:
        //Data transfer
        int sendData(string type, string name, string data);
        int recvData(int sock);
        int putData(string data);
        int getData(string name,string type);
        
        //Compresion stuffs
        string compress(string data, int compresionAlg);
        string uncompress(string compressedData, int compresionAlg);
        
        //Encryption stuffs
        int genKey();
        int initHandshake(int sock);
        int recvHandshake(int sock);
        string encrypt(string data, int encryptionAlg);
        string decrypt(string encryptedData, int encryptionAlg);
        
        //Sqlite3
        int initDB();
        bool doesDBExist();
        bool isDBOpen(string funcName);
        int sqliteExec(string command, bool giveReturn);     //Used to run ANY sqlite3 command
        int sqliteInsert(string command);  //ONLY used to run insert commands, will update database instead of insert if nessisary
        void clearTmpVars();
    public:
        basicNetworking();
        
        //Basic setup
        int setupClient(string ServerAddr, bool dontStop = false, bool eraseOldDB = true);
        int setupServer(bool dontStop = false, bool eraseOldDB = true);
        int usePort(int port);
        void quit();
        int compressData(int compression, bool enable);
        int encryptData(int encryption, bool enable);
        void getInfo();
        int clearDB(string database = DB_NAME);
        void returnZero(bool zero);
        
        //Send data
        int sendChar(string name, char data);
        int sendDouble(string name, double num);
        int sendFloat(string name, float num);
        int sendInt(string name, int num);
        int sendString(string name, string str);
        int sendBool(string name, bool val);
        //int sendOther();
        
        //Recive data
        char getChar(string name);
        double getDouble(string name);
        float getFloat(string name);
        int getInt(string name);
        string getString(string name);
        bool getBool(string name);
       
};

#endif
