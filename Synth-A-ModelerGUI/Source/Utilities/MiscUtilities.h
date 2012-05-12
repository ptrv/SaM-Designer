/*
  ==============================================================================

    MiscUtilities.h
    Created: 12 May 2012 3:38:26pm
    Author:  Peter Vasil

  ==============================================================================

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

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
