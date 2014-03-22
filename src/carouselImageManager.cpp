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

carouselImageManager::~carouselImageManager()
{
    for (auto i : mCaims)
        delete i;
}

void carouselImageManager::setup()
{
    mCenterPos = app::getWindowCenter();
    mHitArea = Rectf(Vec2f(0, app::getWindowHeight()*0.2), Vec2f(app::getWindowWidth(),app::getWindowHeight()*0.8));
    
    vector<fs::path> dirs;
    copy(fs::directory_iterator("/Users/Nick/src/carousel/assets/photos/"), fs::directory_iterator(), back_inserter(dirs));
    sort(dirs.begin(), dirs.end());
    
    for (vector<fs::path>::const_iterator it (dirs.begin()); it != dirs.end(); ++it)
    {   if (fs::is_directory( *it ));
        {
            CarouselImage * caim = new CarouselImage(*it);
            app::console() << *it << endl;
            caim -> setup();
            caim -> setWidth( app::getWindowWidth()/2 );
            caim -> setShouldDraw( false );
            sendOffSide( caim, false );
            mCaims . push_back( caim );
        }
    }
    mCaims[0] -> setShouldDraw(true);
    mCaims[0] -> setShouldDrawText(true);
    mCaims[1] -> setShouldDraw(true);
    mCaims[1] -> setShouldDrawText(true);
}

void carouselImageManager::mouseDown( app::MouseEvent event )
{

}

void carouselImageManager::update()
{
    for (auto i : mCaims)
        i -> update();
}

void carouselImageManager::draw()
{   // draw the images
    for (auto i : mCaims)
        i -> draw();
}

void carouselImageManager::advance()
{
    if ( mCIndex == mCaims.size()-1 )
    {
        app::console() << "can't advance" << endl;
        return;
    }
    Anim<ColorA> * animColor;
    
    if ( caimsIndexCheck( mCIndex-2 ) )
        mCaims[mCIndex-2] -> setShouldDraw( false );
    if ( caimsIndexCheck( mCIndex-1 ) )
        sendOffSide( mCaims[mCIndex-1], true );
    animColor = sendToSide( mCaims[mCIndex], true );
    if ( caimsIndexCheck( mCIndex+1 ) && animColor)
    {
        mCaims[mCIndex+1] -> setPos( mCenterPos );
        mCaims[mCIndex+1] -> setShouldDrawText( true, animColor);
    }
    if (caimsIndexCheck( mCIndex+2 ))
        sendToSide( mCaims[mCIndex+2], false );
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
    
    if (caimsIndexCheck( mCIndex+2 ))
        mCaims[mCIndex+2] -> setShouldDraw( false );
    if ( caimsIndexCheck( mCIndex+1 ) )
        sendOffSide( mCaims[mCIndex+1], false );
    animColor = sendToSide( mCaims[mCIndex], false );
    if ( caimsIndexCheck( mCIndex-1 ) && animColor)
    {
        mCaims[mCIndex-1] -> setPos( mCenterPos );
        mCaims[mCIndex-1] -> setShouldDrawText( true, animColor);
    }
    if ( caimsIndexCheck( mCIndex-2 ) )
        sendToSide( mCaims[mCIndex-2], true );

    mCIndex--;
}

int carouselImageManager::getCurYear() const
{
    return mCaims[mCIndex]->getYear();
}

bool carouselImageManager::hitCheck( Vec2f pt ) const
{
    return mHitArea.contains( pt );
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

bool carouselImageManager::caimsIndexCheck(const int i)
{
    if ( i < 0 )
        return false;
    if ( i > mCaims.size()-1 )
        return false;
    return true;
}