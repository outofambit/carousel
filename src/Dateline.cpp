//
//  Timeline.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/21/14.
//
//

#include "Dateline.h"

using namespace ci;
using namespace std;

Dateline::Dateline()
{}

void Dateline::setup()
{
    float xAxis = 100;
    mStartPt = Vec2f( app::getWindowWidth() * 0.2, xAxis );
    mEndPt = Vec2f( app::getWindowWidth() * 0.8, xAxis );
    
    vector<fs::path> dirs;
    copy(fs::directory_iterator("/Users/Nick/src/carousel/assets/photos/"), fs::directory_iterator(), back_inserter(dirs));
    sort(dirs.begin(), dirs.end());
    
    for (vector<fs::path>::const_iterator it (dirs.begin()); it != dirs.end(); ++it)
    {   if (fs::is_directory( *it ));
        {
            mYears.push_back( std::atoi( it->filename().string().c_str() ) );
        }
    }
    mCurYear = mYears[0];
}

Vec2f Dateline::getPointFromYear(const int y) const
{   // clamping needed. sort of
    return mStartPt + (mEndPt - mStartPt) * (y - mYears[0]) / (mYears[mYears.size()-1] - mYears[0]);
}

int Dateline::getYearFromPoint(const ci::Vec2f pt) const
{   // clamping needed
    return (mYears[mYears.size()-1] - mYears[0]) * ((pt - mStartPt) / (mEndPt - mStartPt)).x;
}

void Dateline::goToYear(const int y)
{
    mCurYear = y;
}

void Dateline::draw()
{
    gl::color(1.0, 1.0, 0.0);
    // draw line
    gl::drawLine(mStartPt, mEndPt);
    // draw hashes
    for (int i = mYears[0] + (10 - mYears[0]%10); i < mYears[mYears.size()-1]; i+=10)
    {
        gl::drawLine( getPointFromYear(i) - Vec2f( 0, 10 ), getPointFromYear(i) + Vec2f( 0, 10 ));
    }
    // draw caret
    gl::drawSolidCircle( getPointFromYear( mCurYear ), 5);
}