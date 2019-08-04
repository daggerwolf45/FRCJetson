/*
 * basicNetworking.cpp, Copyright (c) 2019, daggerwolf45. All rights reserved.
 */
#include "basicNetworking.h"

bool isSetup = false;
int sockfd = 0, valread; 
const char *servIP;

using namespace std;

basicNetworking::basicNetworking(){
    
}


int basicNetworking::setup(string strIP){
    const char *servIP = strIP.c_str();
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
    return 0;
}

/*
 * Send types of data
 */

int basicNetworking::sendChar(char chr){
    if (isSetup) {
        string foo;
        foo.push_back(chr);
        const char *data = foo.c_str();
        send(sockfd , data , strlen(data) , 0);
        return 0;
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

int basicNetworking::sendStr(string str){
    if (isSetup) {
        const char *data = str.c_str();
        if(send(sockfd , data , strlen(data) , 0) < 0);
        return 0;
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

int basicNetworking::sendInt(int num){
    if (isSetup) {
        const char *data = to_string(num).c_str();
        if(send(sockfd , data , strlen(data) , 0) < 0);
        return 0;
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

int basicNetworking::sendFloat(float num){
    if (isSetup) {
        const char *data = to_string(num).c_str();
        if(send(sockfd , data , strlen(data) , 0) < 0);
        return 0;
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}


int basicNetworking::sendDouble(double num){
    if (isSetup) {
        const char *data = to_string(num).c_str();
        if(send(sockfd , data , strlen(data) , 0) < 0);
        return 0;
    } 
    else {
        cout << "BN Error: No server connection found, please run setup" << endl;
        return -1;
    }
}

    //if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    //{ 
    //    printf("\nConnection Failed \n"); 
    //    return -1; 
    //} 
    //send(sock , hello , strlen(hello) , 0 ); 
    //printf("Hello message sent\n"); 
    //valread = read( sock , buffer, 1024); 
    //printf("%s\n",buffer ); 
    //return 0; 
//} 