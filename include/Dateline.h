//
//  Timeline.h
//  carousel
//
//  Created by Nicholas Masso on 3/21/14.
//
//

#ifndef __carousel__Timeline__
#define __carousel__Timeline__

#include "cinder/Vector.h"
#include "cinder/app/App.h"
#include "cinder/Timeline.h"
#include "cinder/gl/Texture.h"
#include <vector>
#include <map>
#include <iostream>

class Dateline {
    ci::ColorA          mColor;
    ci::Vec2f           mStartPt, mEndPt;
    ci::Anim<ci::Vec2f> mCurPt;
    int                 mCurYear;
    std::vector<int>    mYears;
    std::map<int, ci::gl::Texture> mDecTexs;

    int                 getYearFromPoint( const ci::Vec2f pt ) const;
    ci::Vec2f           getPointFromYear( const int y ) const;
    
public:
    Dateline();
    void                setup();
    void                goToYear( const int y );
    void                goToPoint( const ci::Vec2f pt);
    void                setCaretPoint( const ci::Vec2f pt );
    int                 getCurYear() const;
    bool                hitCheck( const ci::Vec2f pt ) const;
    void                draw();
};

#endif /* defined(__carousel__Timeline__) */
