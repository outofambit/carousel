//
//  FingerTracker.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/21/14.
//
//

#include "FingerTracker.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void FingerTracker::setup( carouselImageManager *c )
{
    cim = c;
}

void FingerTracker::touchesBegan( TouchEvent event )
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt )
    {
        mFingers.insert( make_pair( touchIt -> getId(), Finger ( touchIt -> getPos() ) ) );
    }
}

void FingerTracker::touchesMoved( TouchEvent event )
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt )
    {
        mFingers[touchIt->getId()].updatePos( touchIt->getPos() );
    }
}

void FingerTracker::touchesEnded(TouchEvent event)
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {

        if (mFingers[touchIt->getId()].isLeftward() && cim->hitCheck(mFingers[touchIt->getId()].mStartPos))
        {
            cim->advance();
        }
        else if (mFingers[touchIt->getId()].isRightward() && cim->hitCheck(mFingers[touchIt->getId()].mStartPos))
        {
            cim->devance();
        }
		mFingers.erase( touchIt->getId() );
	}
}

void FingerTracker::draw()
{
    for( map<uint32_t,Finger>::const_iterator touchersIt = mFingers.begin(); touchersIt != mFingers.end(); ++touchersIt ) {
		touchersIt->second.draw();
	}
}