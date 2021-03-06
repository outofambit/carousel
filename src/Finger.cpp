//
//  Finger.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/21/14.
//
//

#include "Finger.h"
#include "cinder/gl/gl.h"

using namespace ci;

Finger::Finger()
{}

Finger::Finger ( const Vec2f startPos )
{ mStartPos = mPrevPos = mCurPos = startPos;
}

void Finger::updatePos( const Vec2f latestPos )
{
    mPrevPos = mCurPos;
    mCurPos = latestPos;
}

Vec2f Finger::posChange() const
{ return mCurPos - mPrevPos; }

bool Finger::isLeftward() const
{
    return ( mCurPos-mStartPos ).x < -20;
}

bool Finger::isRightward() const
{
    return ( mCurPos-mStartPos ).x > 20;
}

float Finger::totalYDiff() const
{ return (mCurPos - mStartPos).y; }

float Finger::lastYDiff() const
{ return (mCurPos - mPrevPos).y; }


void Finger::draw () const
{
    // for debugging
    gl::color( 0, 1, 0 );
    gl::drawLine( mStartPos, mCurPos );
    gl::drawSolidCircle( mCurPos, 10 );
}
