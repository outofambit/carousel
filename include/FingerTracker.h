//
//  FingerTracker.h
//  carousel
//
//  Created by Nicholas Masso on 3/21/14.
//
//

#ifndef __carousel__FingerTracker__
#define __carousel__FingerTracker__

#include "Finger.h"
#include "carouselImageManager.h"
#include "Dateline.h"
#include "cinder/app/App.h"
#include <iostream>

class FingerTracker {
    std::map<uint32_t,Finger> mFingers;
    carouselImageManager * cim;
    Dateline * dl;
    uint32_t mDatelineOwnerID;
    
public:
    FingerTracker();
    void setup( carouselImageManager *c, Dateline *d );
    void touchesBegan( ci::app::TouchEvent event );
    void touchesMoved( ci::app::TouchEvent event );
    void touchesEnded( ci::app::TouchEvent event );
    void draw();
};

#endif /* defined(__carousel__FingerTracker__) */
