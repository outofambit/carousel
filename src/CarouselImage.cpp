//
//  CarouselImage.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/12/14.
//
//

#include "CarouselImage.h"

using namespace ci;
using namespace std;

CarouselImage::CarouselImage(const fs::path p)  :
    mBasePath(p),
    mPhotoColor(ColorA(1,1,1,1)),
    mReappearing(false),
    mResizing(false)
{
    mYear = std::atoi( p.filename().string().c_str() );
}

void CarouselImage::setup()
{
    mTexture = gl::Texture( loadImage( mBasePath / fs::path( "photo.jpg" ) ) );
    mBR = Vec2f( mTexture.getWidth(), mTexture.getHeight() );
}

gl::Texture CarouselImage::getTitleTexture()
{
    return loadImage( mBasePath / fs::path( "title.png" ) );
}

gl::Texture CarouselImage::getNamesTexture()
{
    return loadImage( mBasePath / fs::path( "names.png" ) );
}

void CarouselImage::prepToReappear()
{ mReappearing = true;  }

void CarouselImage::setPos(const Vec2f new_pos)
{
    if ( mReappearing )
    {
        app::timeline().apply(&mPhotoColor, ColorA(1,1,1,0), 0.5, EaseNone());
        app::timeline().apply(&mNamesColor, ColorA(1,1,1,0), 0.5, EaseNone());
        app::timeline().apply(&mTitleColor, ColorA(1,1,1,0), 0.5, EaseNone());
        app::timeline().apply(&mPos, new_pos, 0.35f, EaseOutQuint() ). appendTo(&mPhotoColor);
        app::timeline().appendTo(&mPhotoColor, ColorA(1,1,1,1), 0.5, EaseNone()).appendTo(&mPos);
        
        mReappearing = false;
    }
    else
        app::timeline().apply( &mPos, new_pos, 0.35f, EaseOutQuint() );
}

void CarouselImage::incPosNow(const ci::Vec2f amt)
{
    mPos = mPos.value() + amt;
}

void CarouselImage::setWidth(const float width)
{
    const float height = width / mTexture.getAspectRatio();
    app::timeline().apply( &mBR, Vec2f(width, height), 0.25f, EaseInOutQuint());
}

void CarouselImage::setWidthNow(const float new_width)
{
    const float new_height = new_width / mTexture.getAspectRatio();
    mBR = Vec2f( new_width, new_height );
}

float CarouselImage::getWidth()
{
    return mBR.value().x;
}

Anim<ColorA> * CarouselImage::getPhotoAnimColor()
{   return &mPhotoColor;    }

void CarouselImage::update()
{
    mPhotoRect.set(0, 0, mBR.value().x, mBR.value().y);
    mPhotoRect.offsetCenterTo( mPos );
    
}

void CarouselImage::draw()
{
    if (getShouldDraw())
    {
        gl::color(mPhotoColor);
        if ( mTexture )
            gl::draw( mTexture, mPhotoRect );
        
        gl::color(mTitleColor);
        if ( mTitleTex )
            gl::draw(mTitleTex, Vec2f(app::getWindowWidth()/2-mTitleTex.getWidth()/2, 150));
        
        gl::color(mNamesColor);
        if ( mNamesTex )
            gl::draw(mNamesTex, mNamesSrcArea, mNamesRect);
        
    }
}

void CarouselImage::setShouldDraw(const bool b)
{
    if ( b != getShouldDraw() )
    {
        if ( b )
        {
            mTitleTex = getTitleTexture();
            mNamesTex = getNamesTexture();
            mNamesRect.set( 0, 0, mNamesTex.getWidth(), 75 );
            mNamesRect.offsetCenterTo( Vec2f(app::getWindowWidth()/2, 900) );
        }
        
        else
        {
            mTitleTex.reset();
            mNamesTex.reset();
        }
        
        mShouldDraw = b;
    }
}

bool CarouselImage::getShouldDraw()
{
    return mShouldDraw;
}

Anim<ColorA> * CarouselImage::setShouldDrawText( const bool b, Anim<ci::ColorA> * triggerPtr)
{
    if ( b != mShouldDrawText )
    {
        mShouldDrawText = b;
        if ( b )
        {
            mNamesSrcArea.set( 0, 0, mNamesRect.getWidth(), mNamesRect.getHeight() );
            mNamesSrcArea.moveULTo( Vec2f(0,0) );
            if ( triggerPtr )
            {
                app::timeline().apply( &mTitleColor, ColorA(1,1,1,1), 0.5f, EaseInOutQuint()).appendTo( triggerPtr );
                app::timeline().apply( &mNamesColor, ColorA(1,1,1,1), 0.5f, EaseInOutQuint()).appendTo( &mTitleColor );
                return &mNamesColor;
            }
            else
            {
                app::timeline().apply( &mTitleColor, ColorA(1,1,1,1), 0.5f, EaseInOutQuint() );
                app::timeline().apply( &mNamesColor, ColorA(1,1,1,1), 0.5f, EaseInOutQuint()).appendTo( &mTitleColor );
                return &mNamesColor;
            }

        }
        else
        {
            app::timeline().apply( &mTitleColor, ColorA(1,1,1,0), 0.5f, EaseInOutQuint());
            app::timeline().apply( &mNamesColor, ColorA(1,1,1,0), 0.5f, EaseInOutQuint());
            return &mTitleColor;
        }

    }
    return NULL;
}

bool CarouselImage::hitCheck( const Vec2f pt ) const
{ return mPhotoRect.contains( pt ); }

bool CarouselImage::namesHitCheck(const ci::Vec2f pt) const
{ return mNamesRect.contains(pt); }

void CarouselImage::resizePhoto( const float inflate_amt )
{
    if (! mResizing)
        mOriginalWidth = mPhotoRect.getWidth();
    setWidthNow( getWidth() + inflate_amt );
    mResizing = true;
}

void CarouselImage::resetPhotoSize()
{
    setWidth(mOriginalWidth);
    mResizing = false;
}

void CarouselImage::offsetNamesArea( Vec2f amt )
{
    if ( mNamesSrcArea.getLR().y + amt.y >= mNamesTex.getHeight() )
        amt.y = mNamesSrcArea.getLR().y - mNamesTex.getHeight();
    else if ( mNamesSrcArea.getUL().y + amt.y <= 0 )
        amt.y = -mNamesSrcArea.getUL().y;
    mNamesSrcArea.offset( amt );
}

int CarouselImage::getYear() const
{ return mYear; }