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
    if (mResetTracking && mFingers.size() > 1) {
        mStartPos = mCurPos = mPrevPos = (mFingers[0]->mCurPos + mFingers[1]->mCurPos) / 2;
        mStartSpread = mCurSpread = mPrevSpread = mFingers[0]->mCurPos.distance(mFingers[1]->mCurPos);
        mResetTracking = false;
    }
    else if (mFingers.size() > 1) {
        mPrevPos = mCurPos;
        mCurPos = (mFingers[0]->mCurPos + mFingers[1]->mCurPos) / 2;
        mPrevSpread = mCurSpread;
        mCurSpread = mFingers[0]->mCurPos.distance(mFingers[1]->mCurPos);
    }
    else {
        mStartPos = mCurPos = mPrevPos = Vec2f (0,0);
        mStartSpread = mCurSpread = mPrevSpread = 0;
    }
}

bool Pincher::hasFinger (Finger const * finger) const
{
    for( vector<Finger *>::const_iterator fingIt = mFingers.cbegin(); fingIt != mFingers.cend(); ++fingIt ) {
        if (*fingIt == finger)
            return true;
    }
    return false;
}

bool Pincher::usingFinger(const Finger *finger) const
{
    for( vector<Finger *>::const_iterator fingIt = mUsingFingers.cbegin(); fingIt != mUsingFingers.cend(); ++fingIt ) {
        if (*fingIt == finger)
            return true;
    }
    return false;
}

Vec2f Pincher::posChange() const
{
    return mCurPos - mPrevPos;
}

float Pincher::spreadChange() const
{
    return mCurSpread - mPrevSpread;
}

void Pincher::addFinger( Finger *f )
{
    mFingers.push_back (f);
    if (mFingers.size() == 2) {
        mResetTracking = true;
        mUsingFingers.push_back(mFingers[0]);
        mUsingFingers.push_back(mFingers[1]);
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
    if (mFingers.size() == 0)
        mUsingFingers.clear();
}