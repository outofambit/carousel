#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "CarouselImage.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class carouselApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    vector<CarouselImage>      cimmys;
};

void carouselApp::setup()
{
//    setFullScreen(true);
    
    vector<fs::path> v;
    copy(fs::directory_iterator("/Users/Nick/src/carousel/assets/"), fs::directory_iterator(), back_inserter(v));
    sort(v.begin(), v.end());
    
    for (vector<fs::path>::const_iterator it (v.begin()); it != v.end(); ++it)
    {   if (fs::is_regular_file( *it ));
        {
            CarouselImage cim = CarouselImage();
            console() << *it << endl;
            cim.setup( *it );
            cim.setPos( getWindowCenter() );
            cim.setWidth( getWindowWidth()/2 );
            cimmys . push_back( cim );
        }
    }
    
}

void carouselApp::mouseDown( MouseEvent event )
{
}

void carouselApp::update()
{
    for (vector<CarouselImage>::iterator it (cimmys.begin()); it != cimmys.end(); ++it)
        it -> update();

}

void carouselApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    for (vector<CarouselImage>::iterator it (cimmys.begin()); it != cimmys.end(); ++it)
        it -> draw();
}

CINDER_APP_NATIVE( carouselApp, RendererGl )
