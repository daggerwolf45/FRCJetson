#include "vPipeline.h"

using namespace std;

int debug = 0;
int contourcount = 0;
bool setZero = true;
double brx[5];
double bry[5];
double brw[2];
double brh[2];
double cx[2];
double cy[2];
double tcx = 0.0;
double tcy = 0.0;

basicNetworking bn;

vPipeline::vPipeline(){
    
}

int main( int argc, char *argv[] ){
    
  // handle command line arguments
    if( ( argc > 1 ) && ( strcmp( argv[1], "--debug" ) == 0 ) )
        debug = 1;

  //Setup Image Pipeline
    cout << "Setting up pipeline (1/2)" << endl;
    cv::Mat img;
    grip::GripPipeline vPipeline;
    cv::VideoCapture input(0);

  
    //Setup NetworkTables
    cout << "Setting up networking (2/2)" << endl;
    bn.setupClient(robotIP);
    cout << "Finished Setup" << endl;
   
    cout << "Starting Pipeline" << endl;
  
    for (;;){
        // STEP 1: fetch image
        if(!input.read(img))
        break;
  	
        // STEP 2: setup image pipeline
        //vPipeline.setsource0(img);
        vPipeline.Process(img);
        
        // STEP 3: obtain intermediate images and countour vectors
        std::vector<std::vector<cv::Point> >* img_filtercontours = vPipeline.GetFilterContoursOutput();
		
        cout << "Attempting to find centerX and centerY of each contour" << endl;
        contourcount = 0;
        for (std::vector<cv::Point> contour: *img_filtercontours){
            contourcount++;
            cout << "Light tape " << contourcount << " (x,y):(w,h)" << endl;
            cv::Rect br = boundingRect(contour);
      
            cout << "(" << br.x << "," << br.y << "):(" << br.width << "," << br.height << ")" << endl; 
      
            brx[contourcount] = br.x;
            bry[contourcount] = br.y;
            brw[contourcount] = br.width;
            brh[contourcount] = br.height;
        }
    
        cout << "contourcount = " << contourcount << endl;
       

        if(contourcount == 2) {
            cx[1] = brx[1]+brw[1]/2;
            cy[1] = bry[1]+brh[1]/2;
            cx[2] = brx[2]+brw[2]/2;
            cy[2] = bry[2]+brh[2]/2;

            tcx = (cx[1]+cx[2])/2;
            tcy = (cy[1]+cy[2])/2;
            cout << "Center of BOTH pieces of tape! (x,y)" << endl;
            cout << "(" << tcx << "," << tcy << ")" << endl;
    
            cout << "Sending center to robot";
            bn.sendDouble("centerX", tcx);
            bn.sendDouble("centerY", tcy);
            setZero = false;
        } else if(!setZero){
            bn.sendDouble("centerX", 0.0);
            bn.sendDouble("centerY", 0.0);
            setZero = true;
        }

    }
  

}



