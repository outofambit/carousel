//
//  Pincher.cpp
//  carousel
//
//  Created by Nicholas Masso on 4/13/14.
//
//

#include "Pincher.h"

using namespace ci;
using namespace std;

Pincher::Pincher()  :
    mStartSpread (0),
    mCurSpread (0),
    mPrevSpread (0),
    mStartPos (Vec2f(0,0)),
    mCurPos (Vec2f(0,0)),
    mPrevPos (Vec2f(0,0)),
    mResetTracking (false)
{}

void Pincher::update()
{
    if (mResetTracking && mFinger.size() > 1) {
        mStartPos = mCurPos = mPrevPos = (mFingers[0]->mCurPos + mFingers[1]->mCurPos) / 2
        mStartSpread = mCurSpread = mPrevSpread = mFingers[0]->mCurPos.distance(mFingers[1]->mCurPos);
        mResetTracking = false;
    }
    else {
        
    }
}

bool Pincher::hasFinger (Finger const * finger) const
{
    for( vector<Finger *>::iterator fingIt = mFingers.begin(); fingIt != mFingers.end(); ++fingIt ) {
        if (*fingIt == finger)
            return true;
    }
    return false;
}

Vec2f Pincher::posChange() const
{
    update();
    return mCurPos - mPrevPos;
}

float Pincher::spreadChange() const
{
    update();
    return mCurSpread - mPrevSpread;
}

void Pincher::addFinger( Finger *f )
{
    mFingers.push_back (f);
    if (mFingers.size() == 2) {
        mResetTracking = true;
    }
}

void Pincher::removeFinger( Finger *f )
{
    vector<Finger *>::iterator to_delete = mFingers.end();
    for( vector<Finger *>::iterator fingIt = mFingers.begin(); fingIt != mFingers.end(); ++fingIt ) {
        if (*fingIt == f)
            to_delete = fingIt;
    }
    if ( to_delete != mFingers.end() )
        mFingers.erase(to_delete);
}