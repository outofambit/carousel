//
//  BigImageLoader.cpp
//  carousel
//
//  Created by Nicholas Masso on 5/1/14.
//
//

#include "BigImageLoader.h"

using namespace ci;
using namespace std;

BigImageLoader * BigImageLoader::bil = NULL;

BigImageLoader * BigImageLoader::getBil()
{ return bil; }

ci::ConcurrentCircularBuffer<ci::Surface> * BigImageLoader::getSurfaces()
{ return mSurfaces; }

BigImageLoader::BigImageLoader()
{ bil = this; }

void BigImageLoader::setup()
{
    mSurfaces = new ConcurrentCircularBuffer<Surface>( 1 );
}

void BigImageLoader::shutdown()
{
    mSurfaces->cancel();
    if (mThread)
        mThread->join();
}

void BigImageLoader::threadFunc (fs::path imagePath)
{
    ThreadSetup threadSetup;
    mSurfaces->pushFront( loadImage( imagePath ) );
}

void BigImageLoader::loadBigImage (fs::path imagePath)
{
    if (mThread)
        mThread->join();
    mThread = shared_ptr<thread>( new thread( bind( &BigImageLoader::threadFunc, this, imagePath ) ) );
}