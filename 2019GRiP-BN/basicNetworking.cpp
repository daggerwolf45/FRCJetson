#include "basicNetworking.h"

bool isSetup = false;
int sock = 0, valread; 

using namespace std;

basicNetworking::basicNetworking(){
    
}


int basicNetworking::setup(string strIP){
    cout << strIP << endl;
    
    //TODO string to *char conversion working
    //const char *servIP = strIP;
    
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    
    if ((sock = socket(BN_AFIP, BN_PROT, 0)) < 0)   //Make sure socket is valid
    { 
        printf("Socket creation error \n"); 
        return -1; 
    } 
    
    serv_addr.sin_family = BN_AFIP;                 //Set ipv4/ipv6
    serv_addr.sin_port = htons(PORT);               //Set port
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)          //Convert ip addr to binary
    { 
        printf("Invalid address/ Address not supported \n"); 
        return -1; 
    } 
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)    //Connect to robot
    { 
        printf("Could not connect to the robot :( \n"); 
        return -1; 
    } 
    
    isSetup = true;
    return sock;
}

int sendChar(char data){
    return 0;
}

int basicNetworking::sendDouble(double num){
    if (isSetup) {
        const char *data = "Test";
        send(sock , data , strlen(data) , 0);
        cout << "Data sent \n";
        return 0;
    } 
    else {
        cout << "Error: could not send data \n";
        return -1;
    }
}


   
//int main(int argc, char const *argv[]) 
//{ 
    //int sock = 0, valread; 
    //struct sockaddr_in serv_addr; 
    //NOTE char *hello = "Hello from client"; 
    //char buffer[1024] = {0}; 
    //if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    //{ 
    //    printf("\n Socket creation error \n"); 
    //    return -1; 
    //} 
   
    //serv_addr.sin_family = AF_INET; 
    //serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    //if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    //{ 
     //   printf("\nInvalid address/ Address not supported \n"); 
    //    return -1; 
    //} 
   
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