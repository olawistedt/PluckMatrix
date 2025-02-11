
#include <TargetConditionals.h>
#if TARGET_OS_IOS == 1
#import <UIKit/UIKit.h>
#else
#import <Cocoa/Cocoa.h>
#endif

#define IPLUG_AUVIEWCONTROLLER IPlugAUViewController_vPluckMatrix
#define IPLUG_AUAUDIOUNIT IPlugAUAudioUnit_vPluckMatrix
#import <PluckMatrixAU/IPlugAUViewController.h>
#import <PluckMatrixAU/IPlugAUAudioUnit.h>

//! Project version number for PluckMatrixAU.
FOUNDATION_EXPORT double PluckMatrixAUVersionNumber;

//! Project version string for PluckMatrixAU.
FOUNDATION_EXPORT const unsigned char PluckMatrixAUVersionString[];

@class IPlugAUViewController_vPluckMatrix;
