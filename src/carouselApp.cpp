#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "CarouselImage.h"
#include "carouselImageManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;


struct Toucher {
    Vec2f mStartPos, mCurPos;
    Toucher()
    {}
    Toucher (const Vec2f &startPos)
    { mStartPos = startPos; }
    
    void updatePos( const Vec2f &latestPos )
    { mCurPos = latestPos; }
    
    bool isLeftward()
    {
        return (mCurPos-mStartPos).x < 0;
    }
    
    void draw () const
    {
        gl::color(0, 1, 0);
        gl::drawLine(mStartPos, mCurPos);
    }
};
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
    map<uint32_t,Toucher>	mTouches;
};

void carouselApp::prepareSettings( Settings *settings )
{
	settings->enableMultiTouch();
}

void carouselApp::setup()
{
    setFullScreen(true);
    cim.setup();
}

void carouselApp::touchesBegan( TouchEvent event )
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt )
    {
        mTouches.insert( make_pair( touchIt -> getId(), Toucher ( touchIt -> getPos() ) ) );
    }
}

void carouselApp::touchesMoved( TouchEvent event )
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt )
    {
        mTouches[touchIt->getId()].updatePos( touchIt->getPos() );
    }
}

void carouselApp::touchesEnded(TouchEvent event)
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
        if (mTouches[touchIt->getId()].isLeftward() )
        {
            cim.advance();
        }
        else
        {
            cim.devance();
        }
		mTouches.erase( touchIt->getId() );
	}
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
	gl::clear( Color( 0, 0, 0 ) );
    cim.draw();
    for( map<uint32_t,Toucher>::const_iterator touchersIt = mTouches.begin(); touchersIt != mTouches.end(); ++touchersIt ) {
		touchersIt->second.draw();
	}
        
}

CINDER_APP_NATIVE( carouselApp, RendererGl )
