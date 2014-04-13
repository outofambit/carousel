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

FingerTracker::FingerTracker()
{}

void FingerTracker::setup( carouselImageManager *c, Dateline *d )
{
    cim = c;
    dl = d;
}

void FingerTracker::touchesBegan( TouchEvent event )
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt )
    {
        mFingers.insert( make_pair( touchIt -> getId(), Finger ( touchIt -> getPos() ) ) );
        // dateline control tracking
        if ( dl->hitCheck( touchIt->getPos() ) )
            mDatelineOwnerID = touchIt->getId();
        // names image scrolling initial hit check
        if ( cim->getCenterCaIm()->namesHitCheck( touchIt->getPos() ) )
            mNamesOwnerID = touchIt->getId();
    }
}

void FingerTracker::touchesMoved( TouchEvent event )
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt )
    {
        mFingers[touchIt->getId()].updatePos( touchIt->getPos() );
        //dateline
        if (touchIt->getId() == mDatelineOwnerID) {
            dl->setCaretPoint(touchIt->getPos());
        }
        // names image scrolling
        if (touchIt->getId() == mNamesOwnerID) {
            cim->getCenterCaIm()->offsetNamesArea( Vec2f( 0, -mFingers[touchIt->getId()].lastYDiff() ) );
        }
    }
}

void FingerTracker::touchesEnded(TouchEvent event)
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
        
        // carousel advancement/devancement
        if (mFingers[touchIt->getId()].isLeftward() && cim->hitCheck(mFingers[touchIt->getId()].mStartPos))
        {
            cim->advance();
            dl->goToYear(cim->getCurYear());
        }
        else if (mFingers[touchIt->getId()].isRightward() && cim->hitCheck(mFingers[touchIt->getId()].mStartPos))
        {
            cim->devance();
            dl->goToYear(cim->getCurYear());
        }
        
        //dateline
        if ( touchIt->getId() == mDatelineOwnerID ) {
            dl->goToPoint( touchIt->getPos() );
            cim->goToYear( dl->getCurYear() );
            mDatelineOwnerID = NULL;
        }
        
        if ( touchIt->getId() == mNamesOwnerID )
            mNamesOwnerID = NULL;
        
		mFingers.erase( touchIt->getId() );
	}
}

void FingerTracker::draw()
{
    for( map<uint32_t,Finger>::const_iterator touchersIt = mFingers.begin(); touchersIt != mFingers.end(); ++touchersIt ) {
		touchersIt->second.draw();
	}
}