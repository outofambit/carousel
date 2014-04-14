//
//  Pincher.h
//  carousel
//
//  Created by Nicholas Masso on 4/13/14.
//
//

#ifndef __carousel__Pincher__
#define __carousel__Pincher__

#include "cinder/Vector.h"
#include "Finger.h"

class Pincher {
    std::vector<Finger *> mFingers;
    float mStartSpread, mCurSpread, mPrevSpread;
    ci::Vec2f mStartPos, mCurPos, mPrevPos;
    bool mResetTracking;
    
    void update();
    
public:
    Pincher();
    bool hasFinger (Finger const * finger) const;
    ci::Vec2f posChange() const;
    float spreadChange() const;
    void addFinger(Finger *f);
    void removeFinger(Finger *f);
};

#endif /* defined(__carousel__Pincher__) */
