/*
  ==============================================================================

    MiscUtilities.h
    Created: 12 May 2012 3:38:26pm
    Author:  peter

  ==============================================================================
*/

#ifndef __MISCUTILITIES_H_64FF5DE5__
#define __MISCUTILITIES_H_64FF5DE5__

namespace Utils
{

bool cancelAnyModalComponents();

void SAMLogger(const String& message);

FileLogger* getLogger();

void openHelpUrl();

void openDataDir();

};

#endif  // __MISCUTILITIES_H_64FF5DE5__
