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
        else if ( cim->getCenterCaIm()->namesHitCheck( touchIt->getPos() ) )
            mNamesOwnerID = touchIt->getId();
        // pinch to zoom init
        else if ( cim->getCenterCaIm()->hitCheck( touchIt->getPos() ) )
            mPhotoOwnerIDs.push_back( touchIt->getId() );
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
    if (mPhotoOwnerIDs.size() == 2)
    { float curPinchSpread = mFingers[mPhotoOwnerIDs[0]].mCurPos.distance(mFingers[mPhotoOwnerIDs[1]].mCurPos);
        
    Vec2f curPinchPos = (mFingers[mPhotoOwnerIDs[0]].mCurPos + mFingers[mPhotoOwnerIDs[1]].mCurPos) / 2;
      if (mLastPinchSpread != 0)
      {
          cim->getCenterCaIm()->resizePhoto( curPinchSpread-mLastPinchSpread );
          cim->getCenterCaIm()->incPosNow( curPinchPos-mLastPinchPos );
      }
        mLastPinchSpread = curPinchSpread;
        mLastPinchPos = curPinchPos;
        
    }
}

void FingerTracker::touchesEnded(TouchEvent event)
{
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
        
        // carousel advancement/devancement
        if (mPhotoOwnerIDs.size() < 2)
        {
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
        }
        //dateline
        if ( touchIt->getId() == mDatelineOwnerID ) {
            dl->goToPoint( touchIt->getPos() );
            cim->goToYear( dl->getCurYear() );
            mDatelineOwnerID = NULL;
        }
        
        if ( touchIt->getId() == mNamesOwnerID )
            mNamesOwnerID = NULL;
        //photo pinch tracking
        vector<uint32_t>::iterator to_delete = mPhotoOwnerIDs.end();
        for( vector<uint32_t>::iterator idIt = mPhotoOwnerIDs.begin(); idIt != mPhotoOwnerIDs.end(); ++idIt ) {
            if (*idIt == touchIt->getId())
                    to_delete = idIt;
            }
        if ( to_delete != mPhotoOwnerIDs.end() )
        {
            mPhotoOwnerIDs.erase(to_delete);
            mLastPinchPos = Vec2f( 0,0 );
            mLastPinchSpread = 0;
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