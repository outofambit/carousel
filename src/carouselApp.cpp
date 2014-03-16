#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "CarouselImage.h"
#include "carouselImageManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class carouselApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    carouselImageManager cim;
};

void carouselApp::setup()
{
//    setFullScreen(true);
    cim.setup();
}

void carouselApp::mouseDown( MouseEvent event )
{
    cim.mouseDown(event);
}

void carouselApp::update()
{
    cim.update();

}

void carouselApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    cim.draw();
}

CINDER_APP_NATIVE( carouselApp, RendererGl )
