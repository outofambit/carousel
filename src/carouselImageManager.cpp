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
    setCenterPhoto(0);
}

void carouselImageManager::update()
{
    for (vector<CarouselImage>::iterator it (cimmys.begin()); it != cimmys.end(); ++it)
        it -> update();
}

void carouselImageManager::draw()
{
    for (vector<CarouselImage>::iterator it (cimmys.begin()); it != cimmys.end(); ++it)
        it -> draw();
}

void carouselImageManager::setCenterPhoto(const int c)
{
    
    cimmys[c].setPos(mCenterPos);
    cimmys[c].setShouldDraw(true);
    if (c > 0)
        sendToLeft(&cimmys[c-1]);
    if (c < cimmys.size())
        sendToRight(&cimmys[c+1]);

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
    lp += Vec2f( ( caim -> getWidth() / 2 - 10) * d, 0 );
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