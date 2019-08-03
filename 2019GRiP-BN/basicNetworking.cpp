#include "basicNetworking.h"

bool isSetup = false;

using namespace std;

bn::bn(){
    
}

void getInAddr(struct sockaddr *sa){
    
}

int bn::setup(string data){
    cout << data;
    
    isSetup = true;
    return 0;
}

int bn::sendDouble(double foo){
    if (isSetup) {
        cout << foo;
        return 0;
    } 
    else {
        return 1;
    }
}