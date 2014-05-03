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
    mTimeOfLastTouch = getElapsedSeconds();
}

void FingerTracker::update()
{
    if ( getElapsedSeconds() - mTimeOfLastTouch > 300) {
        dl->goToPoint( getWindowCenter() );
        cim->goToYear( dl->getCurYear() );
        mTimeOfLastTouch = getElapsedSeconds();
    }
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
        else if ( cim->getCenterCaIm()->namesHitCheck( touchIt->getPos() ) )
            mNamesOwnerID = touchIt->getId();
        // pinch to zoom init
        else if ( cim->getCenterCaIm()->hitCheck( touchIt->getPos() ) )
            pr.addFinger( &mFingers[touchIt->getId()] );
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
        // move zoomed photo with one finger
        if (cim->getCenterCaIm()->getResizing() && pr.hasFinger(&mFingers[touchIt->getId()]) && ! pr.usingFinger(&mFingers[touchIt->getId()]))
            cim->getCenterCaIm()->incPosNow(mFingers[touchIt->getId()].posChange());
    }

    pr.update();
    cim->getCenterCaIm()->resizePhoto( pr.spreadChange() );
    cim->getCenterCaIm()->incPosNow( pr.posChange() );
    
    mTimeOfLastTouch = getElapsedSeconds();
}

void FingerTracker::touchesEnded(TouchEvent event)
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
        
        // only let single fingers interact that haven't been claimed by pincher
        if ( ! cim->getCenterCaIm()->getResizing() && ! pr.usingFinger(&mFingers[touchIt->getId()]) ) {
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
            //names scrolling
            if ( touchIt->getId() == mNamesOwnerID )
                mNamesOwnerID = NULL;
        }
        //photo pinch tracking
        pr.removeFinger( &mFingers[touchIt->getId()] );
        if ( !pr.usingAnyFingers() )
            cim->getCenterCaIm()->updateResizeRange();
        
		mFingers.erase( touchIt->getId() );
	}
}

void FingerTracker::draw()
{
    for( map<uint32_t,Finger>::const_iterator touchersIt = mFingers.begin(); touchersIt != mFingers.end(); ++touchersIt ) {
		touchersIt->second.draw();
	}
}