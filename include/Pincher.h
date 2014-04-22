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
    std::vector<Finger *> mUsingFingers;
    float mStartSpread, mCurSpread, mPrevSpread;
    ci::Vec2f mStartPos, mCurPos, mPrevPos;
    bool mResetTracking;
    
public:
    Pincher();
    void update();
    bool hasFinger (Finger const * finger) const;
    bool usingFinger (Finger const * finger) const;
    bool usingAnyFingers () const;
    ci::Vec2f posChange() const;
    float spreadChange() const;
    void addFinger(Finger *f);
    void removeFinger(Finger *f);
};

#endif /* defined(__carousel__Pincher__) */
