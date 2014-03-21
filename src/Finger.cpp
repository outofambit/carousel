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

Finger::Finger ( const Vec2f &startPos )
{ mStartPos = startPos; }

void Finger::updatePos( const Vec2f &latestPos )
{ mCurPos = latestPos; }

bool Finger::isLeftward() const
{
    return ( mCurPos-mStartPos ).x < -20;
}

bool Finger::isRightward() const
{
    return ( mCurPos-mStartPos ).x > 20;
}

void Finger::draw () const
{
    // for debugging
    gl::color( 0, 1, 0 );
    gl::drawLine( mStartPos, mCurPos );
}
