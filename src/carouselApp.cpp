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
    
    CarouselImage cimg;
};

void carouselApp::setup()
{
    std::cerr << app::getAssetPath( "msi.jpg" ) << std::endl;
    cimg.setup();
}

void carouselApp::mouseDown( MouseEvent event )
{
}

void carouselApp::update()
{
    cimg.update();
}

void carouselApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    cimg.draw();
}

CINDER_APP_NATIVE( carouselApp, RendererGl )
