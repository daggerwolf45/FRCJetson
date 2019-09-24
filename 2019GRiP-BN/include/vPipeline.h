#ifndef VPIPELINE_H
#define VPIPELINE_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "GripPipeline.h"
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdio>
#include <string>
#include <sstream>

#define robotIP "127.0.0.1"
#define PORT 663

#include "basicNetworking.h"

using namespace std;

class vPipeline {
    private:
        
    public:
        vPipeline();

};

#endif