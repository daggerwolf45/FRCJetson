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

#define robotIP "10.6.63.2"
#define port 663

#include "basicNetworking.h"

using namespace std;

class vPipeline {
    private:
        
    public:
        vPipeline();

};

class testC{
    private:
    
    public:
        void testF(string var);
};

#endif