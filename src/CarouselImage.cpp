//
//  CarouselImage.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/12/14.
//
//

#include "CarouselImage.h"
#include "BigImageLoader.h"

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
    mTexture = getPhotoTexture();
    mBR = Vec2f( mTexture.getWidth(), mTexture.getHeight() );
}

gl::Texture CarouselImage::getPhotoTexture() const
{
    char buffer [15];
    sprintf(buffer, "photo-%i.jpg", mYear);
    return loadImage( mBasePath / fs::path( buffer ) ) ;
}

gl::Texture CarouselImage::getTitleTexture() const
{
    return loadImage( mBasePath / fs::path( "title.png" ) );
}

gl::Texture CarouselImage::getNamesTexture() const
{
    return loadImage( mBasePath / fs::path( "names.png" ) );
}

void CarouselImage::prepToReappear()
{ mReappearing = true;  }

void CarouselImage::setPos(const Vec2f new_pos)
{
    if ( mReappearing )
    {
        app::timeline().apply(&mPhotoColor, ColorA(1,1,1,0), 0.25, EaseNone());
        app::timeline().apply(&mNamesColor, ColorA(1,1,1,0), 0.25, EaseNone());
        app::timeline().apply(&mTitleColor, ColorA(1,1,1,0), 0.25, EaseNone());
        app::timeline().apply( &mPos, new_pos, 0.01f, EaseNone() ). appendTo(&mPhotoColor);
        app::timeline().appendTo(&mPhotoColor, ColorA(1,1,1,1), 0.25, EaseNone()).appendTo(&mPos);
        mShouldDrawText = false;
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

float CarouselImage::getWidth() const
{ return mBR.value().x; }

Anim<ColorA> * CarouselImage::getPhotoAnimColor()
{   return &mPhotoColor;    }

void CarouselImage::update()
{
    mPhotoRect.set(0, 0, mBR.value().x, mBR.value().y);
    mPhotoRect.offsetCenterTo( mPos );
    
}

void CarouselImage::draw()
{
    if (getShouldDraw()) {
        gl::color(mTitleColor);
        if ( mTitleTex )
            gl::draw(mTitleTex, Vec2f(app::getWindowWidth()/2-mTitleTex.getWidth()/2, 150));
        
        gl::color(mNamesColor);
        if ( mNamesTex )
            gl::draw(mNamesTex, mNamesSrcArea, mNamesRect);
        
        if (mResizing) {
            // todo: fade this in
            gl::color(0.0, 0.0, 0.25);
            gl::drawSolidRect( Rectf(0,0,app::getWindowWidth(), app::getWindowHeight()) );
        }
        
        gl::color(mPhotoColor);
        gl::Texture photoTexture;
        if ( mResizing && !mZoomTex && BigImageLoader::getBil()->getSurfaces()->isNotEmpty()) {
            Surface newSurface;
            BigImageLoader::getBil()->getSurfaces()->popBack( &newSurface );
            mZoomTex = gl::Texture( newSurface );
        }
        if ( mZoomTex )
            gl::draw( mZoomTex, mPhotoRect );
        else if ( mTexture )
            gl::draw( mTexture, mPhotoRect );
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

bool CarouselImage::getShouldDraw() const
{ return mShouldDraw; }

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
                app::timeline().apply( &mTitleColor, ColorA(1,1,1,1), 0.3f, EaseInOutQuint()).appendTo( triggerPtr );
                app::timeline().apply( &mNamesColor, ColorA(1,1,1,1), 0.3f, EaseInOutQuint()).appendTo( triggerPtr );
                return &mNamesColor;
            }
            else
            {
                app::timeline().apply( &mTitleColor, ColorA(1,1,1,1), 0.3f, EaseInOutQuint() );
                app::timeline().apply( &mNamesColor, ColorA(1,1,1,1), 0.3f, EaseInOutQuint());
                return &mNamesColor;
            }

        }
        else
        {
            app::timeline().apply( &mTitleColor, ColorA(1,1,1,0), 0.25f, EaseInOutQuint());
            app::timeline().apply( &mNamesColor, ColorA(1,1,1,0), 0.25f, EaseInOutQuint());
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
    if ( inflate_amt == 0 )
        return;
    
    if (! mResizing) {
        mOriginalWidth = mPhotoRect.getWidth();
        mPrevResizeRange = mResizeRange = 0;
        
        char buffer [15];
        sprintf(buffer, "zoom-%i.jpg", mYear);
        BigImageLoader::getBil()->loadBigImage( mBasePath / fs::path( buffer ) );
        
        mResizing = true;
    }
    setWidthNow( getWidth() + inflate_amt );
}

void CarouselImage::resetPhotoSize()
{
    if (mResizing) {
        setWidth(mOriginalWidth);
        mZoomTex.reset();
        mResizing = false;
        setPos( app::getWindowCenter() );
    }
}

bool CarouselImage::getResizing() const
{ return mResizing; }

void CarouselImage::updateResizeRange()
{
    if ( !mResizing )
        return;
    
    mPrevResizeRange = mResizeRange;
    if ( getWidth() > app::getWindowWidth() * 0.9 )
        mResizeRange = 2;
    else if ( getWidth() > mOriginalWidth )
        mResizeRange = 1;
    else
        mResizeRange = 0;
    
    if ( mResizeRange == 1 && mPrevResizeRange == 0 )
        setWidth( app::getWindowWidth() );
    else if ( mResizeRange == 2 && mPrevResizeRange == 1 )
        return;
    else if ( mResizeRange == 1 && mPrevResizeRange == 2 )
        resetPhotoSize();
    else if ( mResizeRange == 0 && (mPrevResizeRange == 1 ||  mPrevResizeRange == 2))
        resetPhotoSize();
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