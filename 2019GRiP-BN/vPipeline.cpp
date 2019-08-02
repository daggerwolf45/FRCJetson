#include "vPipeline.h"

using namespace cv;
using namespace std;
using namespace cs;

RNG rng(12345);
int debug = 0;
int contourcount = 0;
double brx[5];
double bry[5];
double brw[2];
double brh[2];
double cx[2];
double cy[2];
double tcx = 0.0;
double tcy = 0.0;

ICPipeline::ICPipeline(){

}

void PublishNetworkTables(shared_ptr<NetworkTable> table) {
	table->PutNumber("test", 1);
}

int main( int argc, char *argv[] )
{

  // handle command line arguments
    if( ( argc > 1 ) && ( strcmp( argv[1], "--debug" ) == 0 ) )
        debug = 1;
	
  //Setup Image Pipeline
    cout << "Setting up pipeline (1/2)" << endl;
    cv::Mat img;
    grip::GripPipeline ic_pipeline;
    cv::VideoCapture input(0);

  
    //Setup NetworkTables
    cout << "Setting up networktables (2/2)" << endl;
    NetworkTable::SetClientMode();
    NetworkTable::SetTeam(663);
    NetworkTable::SetIPAddress("10.6.63.1/n");
    NetworkTable::Initialize();
    auto visionTable = NetworkTable::GetTable("JETSON");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    cout << "Finished Setup" << endl;
  
  
    cout << "Starting Pipeline" << endl;
  
    for (;;){
        // STEP 1: fetch image
        if(!input.read(img))
        break;
  	
    // STEP 2: setup image pipeline
    //ic_pipeline.setsource0(img);
    ic_pipeline.Process(img);	
    
    // STEP 3: obtain intermediate images and countour vectors
    std::vector<std::vector<cv::Point> >* img_filtercontours = ic_pipeline.GetFilterContoursOutput();
		
    cout <<	"Attempting to find centerX and centerY of each contour" << endl;
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
      	
      	cout << "Updating NetworkTables";
      	//nt->PutNumber( "centerX", tcx );
      	//nt->PutNumber( "centerY", tcy );
        } else {
 	//nt->PutNumber( "centerX", 0 );
      	//nt->PutNumber( "centerY", 0 );
        }

    }
  

}



