#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "CarouselImage.h"
#include "carouselImageManager.h"
#include "FingerTracker.h"
#include "Dateline.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class carouselApp : public AppNative {
  public:
    void prepareSettings( Settings *settings );
	void setup();
    
	void mouseDown( MouseEvent event );
    void keyDown(KeyEvent event);
    void touchesBegan( TouchEvent event );
	void touchesMoved( TouchEvent event );
	void touchesEnded( TouchEvent event );
    
	void update();
	void draw();
    
    carouselImageManager cim;
    FingerTracker ft;
    Dateline dl;
};

void carouselApp::prepareSettings( Settings *settings )
{
	settings->enableMultiTouch();
}

void carouselApp::setup()
{
    setFullScreen(true);
    cim.setup();
    dl.setup();
    ft.setup( &cim, &dl );
}

void carouselApp::touchesBegan( TouchEvent event )
{
    ft.touchesBegan( event );
}

void carouselApp::touchesMoved( TouchEvent event )
{
    ft.touchesMoved( event );
}

void carouselApp::touchesEnded(TouchEvent event)
{
    ft.touchesEnded( event );
}

void carouselApp::keyDown( KeyEvent event )
{
    if ( event.getChar() == 'n' )
        cim.advance();
    else if ( event.getChar() == 'b' )
        cim.devance();
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
	gl::clear( Color( 0.0, 0.0, 0.25 ) );
    gl::enableAlphaBlending();
    dl.draw();
    cim.draw();
    ft.draw();
}

CINDER_APP_NATIVE( carouselApp, RendererGl )
