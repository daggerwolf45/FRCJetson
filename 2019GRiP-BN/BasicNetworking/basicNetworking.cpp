/*
 * basicNetworking.cpp, Copyright (c) 2019, daggerwolf45. All rights reserved.
 */
#include "basicNetworking.h"

bool isClient;
bool isSetup = false;
int sockfd = 0, valread; 
const char *servIP;

using namespace std;

basicNetworking::basicNetworking(){
    
}

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
    serv_addr.sin_port = htons(BN_PORT);               //Set port
    
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
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
    int opt = 1;
    char buffer[1024] = {0};
   
    if ((server_fd = socket(BN_AFIP, BN_PROT, 0)) == 0){ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = BN_AFIP; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(BN_PORT); 
    
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    //Listen for connection
    if (listen(server_fd, 3) < 0){ 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){ 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    
    valread = read( new_socket , buffer, 1024); 
    printf("%s\n",buffer );
    
    isSetup = true;
    isClient = false;
    return 0;
  
    //send(new_socket , hello , strlen(hello) , 0 ); 
    //printf("Hello message sent\n"); 
    //return 0;
}

/*
 * Send types of data
 */

int basicNetworking::sendChar(char chr){
    if(isSetup){
        if(isClient){
            string foo;
            foo.push_back(chr);
            const char *data = foo.c_str();
            send(sockfd , data , strlen(data) , 0);
            return 0;
        }
        else{}
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

int basicNetworking::sendStr(string str){
    if(isSetup){
        if(isClient){
            const char *data = str.c_str();
            if(send(sockfd , data , strlen(data) , 0) < 0);
            return 0;
        }
        else{}
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

int basicNetworking::sendInt(int num){
    if(isSetup){
        if(isClient){
            const char *data = to_string(num).c_str();
            if(send(sockfd , data , strlen(data) , 0) < 0);
            return 0;
        }
        else{}
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

int basicNetworking::sendFloat(float num){
    if(isSetup){
        if(isClient){
            const char *data = to_string(num).c_str();
            if(send(sockfd , data , strlen(data) , 0) < 0);
            return 0;
        }
        else{}
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}


int basicNetworking::sendDouble(double num){
    if(isSetup){
        if(isClient){
            const char *data = to_string(num).c_str();
            if(send(sockfd , data , strlen(data) , 0) < 0);
            return 0;
        }
        else{}
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}