//
//  carouselImageManager.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/13/14.
//
//

#include "carouselImageManager.h"

using namespace ci;
using namespace std;

carouselImageManager::carouselImageManager()
{
    mCIndex = 0;
}

void carouselImageManager::setup()
{
    mCenterPos = app::getWindowCenter();
    
    
    vector<fs::path> v;
    copy(fs::directory_iterator("/Users/Nick/src/carousel/assets/"), fs::directory_iterator(), back_inserter(v));
    sort(v.begin(), v.end());
    
    for (vector<fs::path>::const_iterator it (v.begin()); it != v.end(); ++it)
    {   if (fs::is_regular_file( *it ));
        {
            CarouselImage cim = CarouselImage();
            //            console() << *it << endl;
            cim.setup( *it );
            cim.setWidth( app::getWindowWidth()/2 );
            sendOffSide(&cim, false);
            cim.setShouldDraw(false);
            cimmys . push_back( cim );
        }
    }
}

void carouselImageManager::mouseDown( app::MouseEvent event )
{

}

void carouselImageManager::update()
{
    for (vector<CarouselImage>::iterator it = cimmys.begin(); it != cimmys.end(); ++it)
        it -> update();
}

void carouselImageManager::draw()
{
    for (vector<CarouselImage>::iterator it = cimmys.begin(); it != cimmys.end(); ++it)
        it -> draw();
}

void carouselImageManager::advance()
{
    if ( mCIndex == cimmys.size()-1 )
    {
        app::console() << "can't advance" << endl;
        return;
    }
    
    
    if ( cimmyIndexCheck( mCIndex-2 ) )
        cimmys[mCIndex-2] . setShouldDraw( false );
    if ( cimmyIndexCheck( mCIndex-1 ) )
        sendOffSide( &cimmys[mCIndex-1], true );
    sendToLeft( &cimmys[mCIndex] );
    if ( cimmyIndexCheck( mCIndex+1 ) )
        cimmys[mCIndex+1] . setPos( mCenterPos );
    if (cimmyIndexCheck( mCIndex+2 ))
        sendToRight( &cimmys[mCIndex+2] );
    mCIndex++;
}

void carouselImageManager::devance()
{
    if ( mCIndex == 0 )
    {
        app::console() << "can't devance" << endl;
        return;
    }
    
    if ( cimmyIndexCheck( mCIndex-2 ) )
        sendToLeft( &cimmys[mCIndex-2] );
    if ( cimmyIndexCheck( mCIndex-1 ) )
        cimmys[mCIndex-1].setPos( mCenterPos );
    sendToRight( &cimmys[mCIndex] );
    if ( cimmyIndexCheck( mCIndex+1 ) )
        sendOffSide( &cimmys[mCIndex+1], false );
    if (cimmyIndexCheck( mCIndex+2 ))
        cimmys[mCIndex+2].setShouldDraw( false );
    mCIndex--;
}

void carouselImageManager::sendToLeft(CarouselImage * const caim)
{
    sendToSide(caim, true);
}

void carouselImageManager::sendToRight(CarouselImage * const caim)
{
    sendToSide(caim, false);
}

void carouselImageManager::sendToSide(CarouselImage * const caim, const bool toLeft)
{
    if (! caim)
    {
        app::console() << "no caim to send to side" << endl;
        return;
    }
    int d = 1;
    if ( toLeft )
        d = -1;
    
    Vec2f lp = app::getWindowCenter() + Vec2f( app::getWindowWidth() / 2 * d,0 );
    lp += Vec2f( ( caim -> getWidth() / 2 - 30) * d, 0 );
    caim -> setPos( lp );
    caim -> setShouldDraw(true);
}

void carouselImageManager::sendOffSide(CarouselImage * const caim, const bool toLeft)
{
    if (! caim)
    {
        app::console() << "no caim to send to side" << endl;
        return;
    }
    int d = 1;
    if ( toLeft )
        d = -1;
    
    Vec2f lp = app::getWindowCenter() + Vec2f( (app::getWindowWidth() + caim->getWidth()) * d,0 );
    caim->setPos( lp );
}

bool carouselImageManager::cimmyIndexCheck(const int i)
{
    if ( i < 0 )
        return false;
    if ( i > cimmys.size()-1 )
        return false;
    return true;
}