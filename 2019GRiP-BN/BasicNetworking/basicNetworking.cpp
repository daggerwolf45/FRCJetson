/*
 * basicNetworking.cpp, Copyright (c) 2019, daggerwolf45. All rights reserved.
 */
#include "basicNetworking.h"

bool isClient;
bool isSetup = false;
int userPort = BN_PORT;
string sesKey;

bool databaseSetup = false;
const char *bnpDatabase;

bool useCompression = false;
int compresionAlg;      //(0 = b64pack, 1 = zstd)

bool useEncryption = false;
int encryptionAlg;      //(0 = nes, 1 = rsa, 2 = aes)

int sockfd = 0, valread; 
int new_socket;
const char *servIP;
char buffer[MAXDATASIZE] = {0};

using namespace std;

basicNetworking::basicNetworking(){}
sqlite3::sqlite3(){}

/*
 * Setup client or server
 */

int basicNetworking::setupClient(string strIP){
    const char *servIP = strIP.c_str();
    if(isSetup){
        close(sockfd);
        isSetup = false;
    }
    int sockfd = 0, valread;
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    
    if ((sockfd = socket(BN_AFIP, BN_PROT, 0)) < 0)   //Make sure socket is valid
    { 
        printf("Socket creation error \n"); 
        return -1; 
    } 
    
    serv_addr.sin_family = BN_AFIP;                 //Set ipv4/ipv6
    serv_addr.sin_port = htons(userPort);
    
    if(inet_pton(AF_INET, servIP, &serv_addr.sin_addr) <= 0)          //Convert ip addr to binary
    { 
        printf("Invalid address/ Address not supported \n"); 
        return -1; 
    } 
    
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)    //Connect to robot
    { 
        cout << "Could not connect to server at: " << servIP << endl;
        return -1; 
    } 
    
    isSetup = true;
    isClient = true;
    return 0;
    
   
    //valread = read( sock , buffer, 1024); 
    //printf("%s\n",buffer ); 
    //return 0; 
}

int basicNetworking::setupServer(){
    int serverfd, new_socket, valread; 
    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
    int opt = 1;
   
    if ((serverfd = socket(BN_AFIP, BN_PROT, 0)) == 0){ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = BN_AFIP; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(userPort);
    
    // Forcefully attaching socket to the port 8080 
    if (bind(serverfd, (struct sockaddr *)&address, sizeof(address))<0){ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    //Listen for connection
    if (listen(serverfd, 3) < 0){ 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    
    if ((new_socket = accept(serverfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){ 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    

    
    isSetup = true;
    isClient = false;
    return 0;
    
     
    // printf("%s\n",buffer );
    //send(new_socket , hello , strlen(hello) , 0 ); 
    //printf("Hello message sent\n"); 
    //return 0;
}

int basicNetworking::usePort(int port){
    userPort = port;
    return 0;
}

void basicNetworking::quit(){
    close(sockfd);
}

int basicNetworking::genKey(){
    srand(time(0)*userPort);
    int ikey = rand();
    string sesKey = to_string(ikey);
    return 0;
}

/*
 * Reformat and send/recive data
 */

int basicNetworking::sendData(string type, string name, string data){
    /* NOTE 
     * Sends data in the format: 
     * PROT_VER(int) useCompression(0/1) compresionAlg(int) useEncryption(0/1) encryptionAlg(int) dataType("double") name("coolNumber") data("420.69")
     * 1 0 0 0 0 double coolNumber 420.69
     * 
     * If compression or encryption is used the type, name and data will be compressed first then encrypted
     */
    
    stringstream dataStream;
    stringstream mergeData;
    int curSock;
    
    if (isClient) curSock = sockfd;                                         //Sets socket
    else curSock = new_socket;
    
    mergeData << name << " " << type << " " << data;                        //Combines type, name and data into one string
    data = mergeData.str();                                                 //Sets combined string as data
    
    datastream << PROT_VER << " ";

    if(useCompression){
        data = compress(data, compresionAlg);                               //Compresses data
        dataStream << int(useCompression) << " " << compresionAlg << " ";   //Adds compression and algorithm used, to dataStream
    }
    else {
        dataStream << 0 << " " << 0 << " ";                                 //Adds that no compresion was used, to datasteam
    }
    
    if(useEncryption){
        data = encrypt(data, encryptionAlg);                                //Encrypts data
        dataStream << int(useEncryption) << " " << encryptionAlg << " ";    //Adds encryption and algorithm used, to dataStream
    }
    else {                          
        dataStream << 0 << " " << 0 << " ";                                 //Adds that no encryption was used, to dataStream
    }
    
    dataStream << data;                                                     //Adds data to dataStream
    
    const char *formatedData = dataStream.str().c_str();                    //Formats dataSteam as a char* 
    send(curSock, formatedData, strlen(formatedData), 0);                    //Sends formatedData to open socket
    return 0;
}

int basicNetworking::recvData(int sock){
     /* NOTE 
     * Recives data in the format: 
     * PROT_VER(int) useCompression(0/1) compresionAlg(int) useEncryption(0/1) encryptionAlg(int) dataType("double") name("coolNumber") data("420.69")
     * 1 0 0 0 0 double coolNumber 420.69
     * 
     * If compression or encryption is used, the type, name and data will be decrypted first then uncompressed
     */
    stringstream unformatedData;
    int vers
    bool compresed;
    int compAlg;
    bool encrypted;
    int encryptAlg;
    string rawData;
    string strBuf;
    
    valread = read(sock, buffer, MAXDATASIZE);                              //Get data from network
    unformatedData.str(buffer);                                             //Converts char* to string
    unformatedData >> vers                                                  //Strips protocol version
    if (vers = 1){
        unformatedData >> compresed >> compAlg >> encrypted >> encryptAlg;  //Strips encryption and compression from data
        unformatedData >> rawData;                                          //Strips type into rawData string
        unformatedData >> strBuf; rawData.append(" "+strBuf);               //Strips name into rawData string
        unformatedData >> strBuf; rawData.append(" "+strBuf);               //Strips data into rawData string
    }
    
    if(encrypted){
        rawData = decrypt(rawData, encryptionAlg);                      //Decrypts if encrypted
    }
    
    if(compresed){
        rawData = uncompress(rawData, encryptionAlg);                   //Uncompresses if compressed
    }
    
    putData(rawData);                                                   //Runs putData() so data can be indexed
    return 0;
}

int basicNetworking::putData(string data){
    if (!databaseSetup){
        sqlite3_open(bnpDatabase, );
        databaseSetup = true;
    }
    return 0;
}

/*
 * Choose type of data
 */

int basicNetworking::sendChar(string name, char chr){
    if(isSetup){
        if(isClient){
            string foo;
            foo.push_back(chr);
            sendData("char", name, foo);
            return 0;
        }
        else{return 0;}
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

int basicNetworking::sendStr(string name, string str){
    if(isSetup){
        if(isClient){
            sendData("string", name, str);
            return 0;
        }
        else{return 0;}
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

int basicNetworking::sendInt(string name, int num){
    if(isSetup){
        if(isClient){
            sendData("int", name, to_string(num));
            return 0;
        }
        else{return 0;}
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

int basicNetworking::sendFloat(string name, float num){
    if(isSetup){
        if(isClient){
            sendData("float", name, to_string(num));
            return 0;
        }
        else{return 0;}
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

int basicNetworking::sendDouble(string name, double num){
    if(isSetup){
        if(isClient){
            sendData("double", name, to_string(num));
            return 0;
        }
        else{return 0;}
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

/*
 * Compresion and encryption support            TODO
 */


int basicNetworking::compressData(int alg, bool enable){
    if(!enable){
        useCompression = 0;
        return 0;
    }
    if(useCompression){
        if(alg == compresionAlg){
            return 0;
        }
    }
   
    useCompression = true;
    compresionAlg = alg;
    return 0;
}

string basicNetworking::compress(string data, int compresionAlg){
    //char *foo;
    //size_t foolen = get_message(&foo);
    //b64pack_compress(foo, &foolen);
    
    string compressedData;
    return compressedData;
}

string basicNetworking::uncompress(string compressedData, int compresionAlg){
    string data;
    return data;
}

int basicNetworking::encryptData(int alg, bool enable){
    if(!enable){
        useEncryption = 0;
        return 0;
    }
    if(useEncryption){
        if(alg == encryptionAlg){
            return 0;
        }
    }
   
    useEncryption = true;
    encryptionAlg = alg;
    return 0;
}

string basicNetworking::encrypt(string data, int encryptionAlg){
    string encryptedData;
    return encryptedData;
}

string basicNetworking::decrypt(string encryptedData, int encryptionAlg){
    string data;
    return data;
}


/*
 * Info reporting
 */

void basicNetworking::getInfo(){
    cout << "***********************************************" << endl;
    cout << "BasicNetworkingProtocol v" << BNP_VER " by daggerwolf45" << endl;
    cout << "***********************************************" << endl << endl;
    cout << "Is Setup:           " << isSetup << endl;
    cout << "Client/Server Mode: " << isClient << endl;
    if (!isClient){
        cout << "Is database setup:   " << databaseSetup << endl;
    }
    cout << "Port:               " << userPort << endl;
    cout << "Max Packet Size:    " << MAXDATASIZE << endl;
    cout << "Protocol Formating  " << BNP_VER << endl;
    cout << "IP Protocol         ";
    if (BN_AFIP = AF_INET){
        cout << "IPv4" << endl;
    }
    else if (BN_AFIP = AF_INET6){
        cout << "IPv6" << endl;
    }
    else cout << "N/A";
    cout << "Server IP:           ";
    if (servIP != null){
        << string(servIP) << endl;
    }
    else cout << "N/A" << endl;
    
    cout << endl << "-----------------------------------------------" << endl;
    cout << "Compresion:          ";
    if (useCompression = true){
        if (compresionAlg = 0){
            cout << "b64pack" << endl;
        }
        else if (compresionAlg = 1){
            cout << "zstd" << endl;
        }
    }
    else cout << useCompression << endl;
    cout << "Encryption:          ";
    if (useEncryption){                     //nes rsa aes
        if (encryptionAlg = 0){
            cout << "NES" << endl;
        }
        else if (encryptAlg = 1){
            cout << "RSA 1024bit" << endl;
        }
        else if (encryptAlg = 2){
            cout << "AES 128bit" << endl;
        }
        else cout << "Unknown" << endl;
        cout << "Session Key:         " << sesKey << endl;
    }
    else cout << "False" << endl;
}