#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class carouselApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void carouselApp::setup()
{
}

void carouselApp::mouseDown( MouseEvent event )
{
}

void carouselApp::update()
{
}

void carouselApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( carouselApp, RendererGl )
