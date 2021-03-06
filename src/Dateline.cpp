//
//  Timeline.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/21/14.
//
//

#include "Dateline.h"
#include "cinder/Text.h"

using namespace ci;
using namespace std;

Dateline::Dateline()    :
    mColor(0.54,0.39,0.33,1.0)
{}

void Dateline::setup()
{
    float xAxis = 100;
    mStartPt = Vec2f( 40, xAxis );
    mEndPt = Vec2f( app::getWindowWidth() - 40, xAxis );
    
    vector<fs::path> dirs;
    copy(fs::directory_iterator(app::getAssetPath("photos")), fs::directory_iterator(), back_inserter(dirs));
    sort(dirs.begin(), dirs.end());
    
    for (vector<fs::path>::const_iterator it (dirs.begin()); it != dirs.end(); ++it) {
        if (fs::is_directory( *it ) && ! fs::is_empty( *it ) && it->filename().c_str()[0] != '.') {
            mYears.push_back( std::atoi( it->filename().string().c_str() ) );
        }
    }
    
    for (int i = mYears[0] + (10 - mYears[0]%10); i < mYears[mYears.size()-1]; i+=10)
    {
        TextLayout decText;
        decText.setFont( Font( "Futura", 21 ));
        decText.clear( ColorA (1.0,1.0,1.0,0.0) );
        decText.setColor( mColor );
        decText.addLine( to_string( i ) );

        mDecTexs.insert(make_pair(i, ( gl::Texture( decText.render( true ) ) ) ) );
        
    }
    
    goToYear( mYears[0] );
    
}

Vec2f Dateline::getPointFromYear(const int y) const
{   // clamping needed. sort of
    return mStartPt + (mEndPt - mStartPt) * (y - mYears[0]) / (mYears[mYears.size()-1] - mYears[0]);
}

int Dateline::getYearFromPoint(const ci::Vec2f pt) const
{
    int idealYear = mYears[0] + (mYears[mYears.size()-1] - mYears[0]) * (((pt - mStartPt) / (mEndPt - mStartPt)).x);
    int lowestDiff = 1000000;
    int closestYear = mYears[0];
    for (auto y : mYears)
    {
        if (abs(idealYear-y) < lowestDiff) {
            lowestDiff = abs(idealYear-y);
            closestYear = y;
        }
    }
    return closestYear;
}

void Dateline::goToYear(const int y)
{
    mCurYear = y;
    app::timeline().apply(&mCurPt, getPointFromYear( mCurYear ), 0.25);
}

void Dateline::goToPoint(const Vec2f pt)
{
    goToYear( getYearFromPoint(pt) );
    
}

void Dateline::setCaretPoint(const ci::Vec2f pt)
{
    if (pt.x > mEndPt.x)
        mCurPt.value().x = mEndPt.x;
    else if (pt.x < mStartPt.x)
        mCurPt.value().x = mStartPt.x;
    else
        mCurPt.value().x = pt.x;
}

int Dateline::getCurYear() const
{ return mCurYear; }

bool Dateline::hitCheck(const ci::Vec2f pt) const
{
    if (Rectf(mStartPt+Vec2f(0,-20), mEndPt+Vec2f(0,20)).contains(pt))
        return true;
    
    return false;
}

void Dateline::draw()
{
    gl::color(mColor);
    // draw line
    gl::drawLine(mStartPt, mEndPt);
    // draw hashes
    for (int i = mYears[0] + (10 - mYears[0]%10); i < mYears[mYears.size()-1]; i+=10) {
        gl::color(mColor);
        gl::drawLine( getPointFromYear(i) - Vec2f( 0, 15 ), getPointFromYear(i) );
        gl::color(Color::white());
        gl::draw(mDecTexs[i], getPointFromYear( i ) - Vec2f (mDecTexs[i].getWidth()/2, 0) - Vec2f( 0, mDecTexs[i].getWidth() + 15 ));
    }
    // draw caret
    gl::color(mColor);
    gl::drawSolidTriangle( mCurPt.value()+Vec2f(0,5), mCurPt.value()+Vec2f( -8, 15 ), mCurPt.value()+Vec2f( 8, 15 ));
}