//
//  carouselImageManager.h
//  carousel
//
//  Created by Nicholas Masso on 3/13/14.
//
//

#ifndef __carousel__carouselImageManager__
#define __carousel__carouselImageManager__

#include "CarouselImage.h"
#include <vector>
#include <iostream>

class carouselImageManager {
    std::vector<CarouselImage *>    mCaims;
    ci::Vec2f                       mCenterPos;
    int                             mCIndex;
    ci::Rectf                       mHitArea;
    
public:
    carouselImageManager();
    ~carouselImageManager();
    void setup();
    void update();
    void draw();
    void mouseDown( ci::app::MouseEvent event );
    void setCenterPhoto( const int c );
    void setCenterPhoto( CarouselImage * const caim );
    CarouselImage * getCenterCaIm() const;
    void goToYear (const int y);
    void advance();
    void devance();
    int getCurYear() const;
    bool hitCheck( ci::Vec2f pt ) const;
private:
    void sendCenter( CarouselImage *const caim, ci::Anim<ci::ColorA> * triggerColor = NULL );
    ci::Anim<ci::ColorA> * sendToSide( CarouselImage * const caim, const bool toLeft );
    void sendOffSide( CarouselImage * const caim, const bool toLeft );
    bool caimsIndexCheck( const int i );
};

#endif /* defined(__carousel__carouselImageManager__) */
