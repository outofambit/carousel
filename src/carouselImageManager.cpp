//
//  carouselImageManager.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/13/14.
//
//

#include "carouselImageManager.h"
#include "cinder/app/AppNative.h"
#include "cinder/ImageIo.h"

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
    copy(fs::directory_iterator(app::getAssetPath("photos")), fs::directory_iterator(), back_inserter(dirs));
    sort(dirs.begin(), dirs.end());
    
    for (vector<fs::path>::const_iterator it (dirs.cbegin()); it != dirs.cend(); ++it) {
        if ( fs::is_directory( *it ) && !fs::is_empty( *it ) && it->filename().c_str()[0] != '.') {
            CarouselImage * caim = new CarouselImage(*it);
            caim -> setup();
            caim -> setWidthOrHeight( Vec2f(app::getWindowWidth()*0.75, app::getWindowHeight()/3) );
            caim -> setShouldDraw( false );
            sendOffSide( caim, false );
            mCaims . push_back( caim );
        }
    }
    setCenterPhoto( 0 );
}

void carouselImageManager::update()
{
    for (auto i : mCaims)
        i->update();
}

void carouselImageManager::draw()
{   // draw the images
    for (auto i : mCaims) {
        if (i != mCaims[mCIndex])
            i->draw();
    }
    mCaims[mCIndex]->draw();
    
}

void carouselImageManager::goToYear(const int y)
{
    for ( auto caim : mCaims )
    {
        if ( caim->getYear() == y )
        {
            setCenterPhoto(caim);
            return;
        }
    }
}

void carouselImageManager::setCenterPhoto(CarouselImage * const caim)
{
    for (int i = 0; i < mCaims.size(); i++) {
        if (mCaims[i] == caim)
        {
            setCenterPhoto(i);
            return;
        }
    }
}

void carouselImageManager::setCenterPhoto( const int c )
{
    mCIndex = c;
    for (auto caim : mCaims)
        caim->prepToReappear();
    
    
    for (int i = 0; i < mCaims.size(); i++) {
        if ( i <= mCIndex-2 )
            sendOffSide(mCaims[i], true);
        if ( i == ( mCIndex-1 ) )
            sendToSide(mCaims[i], true);
        if (i == mCIndex)
            sendCenter(mCaims[i], mCaims[i]->getPhotoAnimColor());
        if ( i == ( mCIndex+1 ) )
            sendToSide(mCaims[i], false);
        if ( i >= ( mCIndex+2 ))
            sendOffSide(mCaims[i], false);
    }
}

CarouselImage * carouselImageManager::getCenterCaIm() const
{ return mCaims[mCIndex]; }

int carouselImageManager::getNumOfCaIms() const
{ return mCaims.size(); }

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
        sendCenter( mCaims[mCIndex+1], animColor );
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
        sendCenter( mCaims[mCIndex-1], animColor );
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

void carouselImageManager::sendCenter(CarouselImage *const caim, Anim<ColorA> * triggerColor)
{
    caim->resetPhotoSize();
    caim->setShouldDraw(true);
    caim->setPos( mCenterPos );
    caim->setShouldDrawText( true, triggerColor );
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
    lp += Vec2f( ( caim -> getWidth() / 2 - app::getWindowWidth() * 0.05) * d, 0 );
    caim->resetPhotoSize();
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
    
    Vec2f lp = app::getWindowCenter() + Vec2f( (app::getWindowWidth() + caim->getWidth()) * d, 0 );
    caim->resetPhotoSize();
    caim->setPos( lp );
    caim->setShouldDraw(true);
}

bool carouselImageManager::caimsIndexCheck(const int i)
{
    if ( i < 0 )
        return false;
    if ( i > mCaims.size()-1 )
        return false;
    return true;
}