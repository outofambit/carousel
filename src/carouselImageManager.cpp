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
    
    
    vector<fs::path> dirs;
    copy(fs::directory_iterator("/Users/Nick/src/carousel/assets/photos/"), fs::directory_iterator(), back_inserter(dirs));
    sort(dirs.begin(), dirs.end());
    
    for (vector<fs::path>::const_iterator it (dirs.begin()); it != dirs.end(); ++it)
    {   if (fs::is_directory( *it ));
        {
            CarouselImage cim = CarouselImage(*it);
            app::console() << *it << endl;
            cim.setup();
            cim.setWidth( app::getWindowWidth()/2 );
            sendOffSide( &cim, false );
            cim.setShouldDraw( false );
            cimmys . push_back( cim );
        }
    }
    cimmys[0].setShouldDraw(true);
    cimmys[0].setShouldDrawText(true);
    cimmys[1].setShouldDraw(true);
    cimmys[1].setShouldDrawText(true);
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
{   // draw the images
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
    Anim<ColorA> * animColor;
    
    if ( cimmyIndexCheck( mCIndex-2 ) )
        cimmys[mCIndex-2] . setShouldDraw( false );
    if ( cimmyIndexCheck( mCIndex-1 ) )
        sendOffSide( &cimmys[mCIndex-1], true );
    animColor = sendToSide( &cimmys[mCIndex], true );
    if ( cimmyIndexCheck( mCIndex+1 ) && animColor)
    {
        cimmys[mCIndex+1] . setPos( mCenterPos );
        cimmys[mCIndex+1] . setShouldDrawText( true, animColor);
    }
    if (cimmyIndexCheck( mCIndex+2 ))
        sendToSide( &cimmys[mCIndex+2], false );
    mCIndex++;
}

void carouselImageManager::devance()
{
    if ( mCIndex == 0 )
    {
        app::console() << "can't devance" << endl;
        return;
    }
    
    Anim<ColorA> * animColor;
    
    if (cimmyIndexCheck( mCIndex+2 ))
        cimmys[mCIndex+2].setShouldDraw( false );
    if ( cimmyIndexCheck( mCIndex+1 ) )
        sendOffSide( &cimmys[mCIndex+1], false );
    animColor = sendToSide( &cimmys[mCIndex], false );
    if ( cimmyIndexCheck( mCIndex-1 ) && animColor)
    {
        cimmys[mCIndex-1].setPos( mCenterPos );
        cimmys[mCIndex-1] . setShouldDrawText( true, animColor);
    }
    if ( cimmyIndexCheck( mCIndex-2 ) )
        sendToSide( &cimmys[mCIndex-2], true );

    mCIndex--;
}

Anim<ColorA> * carouselImageManager::sendToSide(CarouselImage * const caim, const bool toLeft)
{
    if (! caim)
    {
        app::console() << "no caim to send to side" << endl;
        return NULL;
    }
    int d = 1;
    if ( toLeft )
        d = -1;
    
    Vec2f lp = app::getWindowCenter() + Vec2f( app::getWindowWidth() / 2 * d,0 );
    lp += Vec2f( ( caim -> getWidth() / 2 - 30) * d, 0 );
    caim -> setPos( lp );
    caim -> setShouldDraw(true);
    return caim -> setShouldDrawText(false);
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