/*
 * basicNetworking.cpp, Copyright (c) 2019, daggerwolf45. All rights reserved.
 */
#include "basicNetworking.h"

bool isClient;
bool isSetup = false;
int userPort = BN_PORT;
bool returnNull = true;
string sesKey;

bool useCompression = false;
int compresionAlg;      //(0 = b64pack, 1 = zstd)

bool useEncryption = false;
int encryptionAlg;      //(0 = nes, 1 = rsa, 2 = aes)

int sockfd = 0, valread; 
int new_socket;
const char *servIP;
char buffer[MAXDATASIZE] = {0};

sqlite3 *db;
bool sqliteFI, databaseOpen = false;
string sqliteFD = "", sqliteFT = "";

using namespace std;

basicNetworking::basicNetworking(){
    
}

/*
 * Setup client or server
 */

int basicNetworking::setupClient(string strIP, bool dontStop, bool eraseOldDB){
    if (eraseOldDB){
        clearDB(DB_NAME);
    }
    initDB();
    
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
    
    if (dontStop){
        bool connected = false;
        while(!connected){
                if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){    //Connect to robot
                    cout << "Could not connect to server at: " << servIP << endl;
                    sleep(3);
            }
        }
    }
    else {
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){    //Connect to robot
            cout << "Could not connect to server at: " << servIP << endl;
            return -1; 
        }
    }
    
    isSetup = true;
    isClient = true;
    return 0;
    
   
    //valread = read( sock , buffer, 1024); 
    //printf("%s\n",buffer ); 
    //return 0; 
}

int basicNetworking::setupServer(bool dontStop, bool eraseOldDB){
    int serverfd, new_socket, valread; 
    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
    int opt = 1;
   
    if ((serverfd = socket(BN_AFIP, BN_PROT, 0)) == 0){ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // FodbErrorefully attaching socket to the port 8080 
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = BN_AFIP; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(userPort);
    
    // FodbErrorefully attaching socket to the port 8080 
    if (bind(serverfd, (struct sockaddr *)&address, sizeof(address))<0){ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    //Listen for connection
    if (listen(serverfd, 4) < 0){ 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    
    if ((new_socket = accept(serverfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){ 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    
    if (eraseOldDB){
        clearDB(DB_NAME);
    }
    
    initDB();
    
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
    isSetup = false;
    close(sockfd);
    
    databaseOpen = false;
    sqlite3_close(db);
}

int basicNetworking::genKey(){
    srand(time(0)*userPort);
    int ikey = rand();
    string sesKey = to_string(ikey);
    return 0;
}

void basicNetworking::returnZero(bool zero){
    returnNull=!zero;
}

/*
 * SQLite Wrapper Functions
 */

int basicNetworking::initDB(){
    int dbError;
    int prevRn = doesDBExist();
   
    dbError = sqlite3_open(DB_NAME, &db);
    if (dbError){
        cout << "Failed to open database: " << sqlite3_errmsg(db) <<endl;
        databaseOpen = false;
        sqlite3_close(db);
    }
    else databaseOpen = true;
    
    if (!prevRn){
        if (sqliteExec("create table BNP1 (id integer primary key, name text not NULL, type text not NULL, data text not NULL)", false)){
            cout << "Could not create BNP1 table" << endl;
            databaseOpen = false;
            sqlite3_close(db);
            return -1;
        }
        
        sqliteInsert("insert into BNP1 (name, type, data) values ('coolNumber', 'double', '420.69')");
        
    }
    
    stringstream text;
    text << "insert into BNP1 (name, type, data) values ('CurTime', 'int', '" << time(0) << "')";
    sqliteInsert(text.str());
    
    return 0;
}

bool basicNetworking::doesDBExist(){
    string name = DB_NAME;
    return ( access( name.c_str(), F_OK) != -1 );
}

bool basicNetworking::isDBOpen(string funcName){
    if(isSetup){
        if (!db || !databaseOpen){
            initDB();
            if (!db || !databaseOpen){
                cout << "DB ERROR: " << funcName << "() unable to locate/open database" << endl;
                return false;
            }
        }
    }
    return true;
}

int basicNetworking::clearDB(string database){
    if (isDBOpen("clearDB")){
        databaseOpen = false;
        sqlite3_close(db);
    }
    
    if (remove(database.c_str())){
        return 0;
    }
    else{
        cout << "BN ERROR: Unable to clear database: " << database << endl;
        return -1;
    }
}

void basicNetworking::clearTmpVars(){
    sqliteFI = false;
    sqliteFD = "";
    sqliteFI = "";
}

int sqliteCallback(void *unused, int count, char **data, char **column){
    if (count > 0){
        sqliteFI = true;
        if (count == 4){
            sqliteFD = data[3];
            sqliteFT = data[2];
        }
        else{
            sqliteFD = "";
        }
    }
    else {
        count = 0;
        sqliteFI = false;
        sqliteFD = "";
        sqliteFI = "";
    }
  
    return 0;
}

int basicNetworking::sqliteExec(string command, bool giveReturn = false){
    char *zErrMsg = 0;
    int dbError;
    
    if (!isDBOpen("sqliteInsert")){
        return -1;
    }
    
    dbError = sqlite3_exec(db, command.c_str(), sqliteCallback, 0, &zErrMsg);
    
   if ( dbError != SQLITE_OK ){
        cout << "SQL ERROR: " << zErrMsg <<endl;
        databaseOpen = false;
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return -1;
    } 
    
    if (giveReturn){
        
    }
    
    return 0;
}

int basicNetworking::sqliteInsert(string command){
    stringstream commandStream;
    string commandHead;
    
    if (!isDBOpen("sqliteInsert")){
        return -1;
    }
    
    commandStream.str(command);
    commandStream >> commandHead;
    
    if (commandHead == "insert" || commandHead == "INSERT"){
        string name, tab, nil, tmp;
        stringstream puller, selectComm;
        
        tmp = command;
        tmp.erase(0,12);
        
        puller.str(tmp);
        puller >> tab;
        
        if (tab == "BNP1"){
            tmp.erase(0, 33);
            
            puller.str(tmp);
            puller >> name;
            
            name.erase(name.end()-2, name.end());
            
        }
        else {
            cout << "BN ERROR: Unknown data format in sqliteInsert()" << endl;
            return -3;
        }
        
        selectComm << "select * from " << tab << " where name = '" << name << "'";
        sqliteExec(selectComm.str(), true);
        /*
         * NOTE This is a VERY NOT PERMANANT CHECK!! This SHOULD just be a return from sqliteExec() or sqliteSelect() NOTE
         */
        if (sqliteFI){
            string type, data;
            stringstream newCommand, partA, partB;
            if (tab == "BNP1"){
                puller >> type >> data;
                
                type.erase(0,1);
                type.erase(type.end()-2, type.end());
                data.erase(0,1);
                data.erase(data.end()-2, data.end());
                
                partA << "update " << tab << " set data = '" << data << "' where name = '" << name << "'; ";
                partA << "update " << tab << " set type = '" << type << "' where name = '" << name << "'";
                newCommand << partA.str() << partB.str();
                command = newCommand.str();
            }
        }
    }
    else {
        cout << "BN ERROR: sqliteInsert() used for non insert command..." << endl;
        return -2;
    }
    
    sqliteExec(command);
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
    if (!isSetup){
        return 1;
    }
    
    stringstream dataStream;
    stringstream mergeData;
    int curSock;
    
    if (isClient) curSock = sockfd;                                         //Sets socket
    else curSock = new_socket;
    
    mergeData << name << " " << type << " " << data;                        //Combines type, name and data into one string
    data = mergeData.str();                                                 //Sets combined string as data
    
    dataStream << PROT_VER << " ";

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
     * PROT_VER(float) useCompression(0/1) compresionAlg(int) useEncryption(0/1) encryptionAlg(int) dataType("double") name("coolNumber") data("420.69")
     * 1 0 0 0 0 double coolNumber 420.69
     * 
     * If compression or encryption is used, the type, name and data will be decrypted first then uncompressed
     */
    stringstream unformatedData;
    float vers;
    bool compresed;
    int compAlg;
    bool encrypted;
    int encryptAlg;
    string rawData;
    string strBuf;
    
    valread = read(sock, buffer, MAXDATASIZE);                              //Get data from network
    unformatedData.str(buffer);                                             //Converts char* to string
    unformatedData >> vers;                                                  //Strips protocol version
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

int basicNetworking::putData(string rawData){
    int dbError;
    string name, type, data;
    stringstream inserts, datastream;
    datastream << rawData;
    
    datastream >> name >> type;
    datastream >> data; //TODO add support for data with spaces...
    
    inserts << "insert into BNP1 (name, type, data) values ('" << name << "', '" << type << "', '" << data << "')";
    if (!isDBOpen("putData")){
        return -1;
    }
    
    sqliteInsert(inserts.str());
    
    return 0;
}

int basicNetworking::getData(string name, string type){
    stringstream command;
    if (!isDBOpen("getData")){
            return -1;
    }
    
    command << "select * from BNP1 where name = '" << name << "'";
    sqliteExec(command.str());
    
    if (sqliteFI){
        if (sqliteFT == type){
            return 0;
        }
        else {
            cout << "DB ERROR: '" << name << "' is of type '" << sqliteFT << "' instead of requested type 'int'\n";
            clearTmpVars();
            return -3;
        }
    }
    else {
        cout << "DB ERROR: Could not get '" << name <<"'. It was not found in database, it may not have been recived yet";
        clearTmpVars();
        return -2;
    }
}

/*
 * Send a type of data
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

int basicNetworking::sendString(string name, string str){
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

int basicNetworking::sendBool(string name, bool val){
    int num = val;
    if(isSetup){
        if(isClient){
            sendData("bool", name, to_string(num));
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
 * Get data from database   TODO
 */

char basicNetworking::getChar(string name){
    char data;
    stringstream stoc;
    
    if (!isSetup){
        cout << "DB ERROR: getChar(), BNP is not currently setup, please run setupClient/Server() first.\n";
        return NULL;
    }
    
    getData(name, "char");
        
    stoc.str(sqliteFD);
    stoc >> data;
    
    clearTmpVars();
    return data;
    
}

string basicNetworking::getString(string name){
    string data;
    
    if (!isSetup){
        cout << "DB ERROR: getString(), BNP is not currently setup, please run setupClient/Server() first.\n";
        return NULL;
    }
   
    getData(name, "string");
    data = sqliteFD;
    
    clearTmpVars();
    return data;
}

bool basicNetworking::getBool(string name){
    bool data, bar;
    stringstream foo;

    if (!isSetup){
        cout << "DB ERROR: getBool(), BNP is not currently setup, please run setupClient/Server() first.\n";
        return NULL;
    }
    
    getData(name, "bool");
    
    foo.str(sqliteFD);
    foo >> bar;
    if (bar){
        data = true;
    }
    else {
        data = false;
    }
    
    clearTmpVars();
    return data;
}

int basicNetworking::getInt(string name){
    int data;
    
    if (!isSetup){
        cout << "DB ERROR: getInt(), BNP is not currently setup, please run setupClient/Server() first.\n";
        return NULL;
    }
   
    getData(name, "int");
    data = stoi(sqliteFD);
    
    clearTmpVars();
    return data;
}

float basicNetworking::getFloat(string name){
    float data;
    
    if (!isSetup){
        cout << "DB ERROR: getFloat(), BNP is not currently setup, please run setupClient/Server() first.\n";
        return NULL;
    }
   
    getData(name, "float");
    data = stof(sqliteFD);
    
    clearTmpVars();
    return data;
}

double basicNetworking::getDouble(string name){
    double data;
    
    if (!isSetup){
        cout << "DB ERROR: getDouble(), BNP is not currently setup, please run setupClient/Server() first.\n";
        return NULL;
    }
   
    getData(name, "double");
    data = stod(sqliteFD);
    
    clearTmpVars();
    return data;
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
    
    string compressedData = data;
    return compressedData;
}

string basicNetworking::uncompress(string compressedData, int compresionAlg){
    string data = compressedData;
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
    string encryptedData = data;
    return encryptedData;
}

string basicNetworking::decrypt(string encryptedData, int encryptionAlg){
    string data = encryptedData;
    return data;
}


/*
 * Info reporting
 */

void basicNetworking::getInfo(){
    cout << "***********************************************" << endl;
    cout << "BasicNetworkingProtocol v" << BNP_VER << " by daggerwolf45" << endl;
    cout << "***********************************************" << endl << endl;
    cout << "Is Setup:           " << isSetup << endl;
    cout << "Client/Server Mode: " << isClient << endl;
    cout << "Port:               " << userPort << endl;
    cout << "Max Packet Size:    " << MAXDATASIZE << endl;
    cout << "Protocol Formating  " << BNP_VER << endl;
    cout << "IP Protocol         ";
    if (BN_AFIP == AF_INET){
        cout << "IPv4" << endl;
    }
    else if (BN_AFIP == AF_INET6){
        cout << "IPv6" << endl;
    }
    else cout << "N/A";
    cout << "Server IP:          ";
    if (servIP != NULL){
        cout << string(servIP) << endl;
    }
    else cout << "N/A" << endl;
    
    cout << endl << "-----------------------------------------------" << endl;
    cout << "Compresion:         ";
    if (useCompression){
        if (compresionAlg = 0){
            cout << "b64pack" << endl;
        }
        else if (compresionAlg = 1){
            cout << "zstd" << endl;
        }
    }
    else cout << useCompression << endl;
    cout << "Encryption:         ";
    if (useEncryption){                     //nes rsa aes
        if (encryptionAlg == 0){
            cout << "NES" << endl;
        }
        else if (encryptionAlg == 1){
            cout << "RSA 1024bit" << endl;
        }
        else if (encryptionAlg == 2){
            cout << "AES 128bit" << endl;
        }
        else cout << "Unknown" << endl;
        cout << "Session Key:         " << sesKey << endl;
    }
    else cout << "False" << endl;
    
    cout << endl << "-----------------------------------------------" << endl;
    cout << "Does database exist:" << bool(doesDBExist()) << endl;
}
