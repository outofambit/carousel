//
//  Finger.h
//  carousel
//
//  Created by Nicholas Masso on 3/21/14.
//
//

#ifndef __carousel__Finger__
#define __carousel__Finger__

#include "cinder/Vector.h"
#include <iostream>

class Finger {
    
    public:
    
    ci::Vec2f mStartPos, mCurPos;
    
    Finger();
    
    Finger( const ci::Vec2f startPos );
    
    void updatePos( const ci::Vec2f latestPos );
    
    bool isLeftward() const;
    
    bool isRightward() const;
    
    void draw () const;
};

#endif /* defined(__carousel__Finger__) */
