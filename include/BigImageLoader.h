//
//  BigImageLoader.h
//  carousel
//
//  Created by Nicholas Masso on 5/1/14.
//
//

#ifndef __carousel__BigImageLoader__
#define __carousel__BigImageLoader__

#include "cinder/app/AppNative.h"
#include "cinder/ImageIo.h"
#include "cinder/Thread.h"
#include "cinder/ConcurrentCircularBuffer.h"
#include "cinder/Surface.h"
#include <iostream>

class BigImageLoader
{
    static BigImageLoader *bil;
    ci::ConcurrentCircularBuffer<ci::Surface>	*mSurfaces;
    std::shared_ptr<std::thread>                mThread;
    
    void threadFunc (ci::fs::path imagePath);
    
public:
    BigImageLoader();
    void setup();
    void shutdown();
    void loadBigImage( ci::fs::path imagePath );
    ci::ConcurrentCircularBuffer<ci::Surface> * getSurfaces();
    static BigImageLoader * getBil();
};

#endif /* defined(__carousel__BigImageLoader__) */
