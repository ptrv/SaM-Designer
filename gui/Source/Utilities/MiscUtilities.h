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

namespace synthamodeler
{
class BaseObjectComponent;
class ObjController;
class MDLFile;
/**
 * Namespace for
 */
namespace Utils
{

/**
 * Cancels any modal window.
 * @return		true if any modal components were closed.
 */
bool cancelAnyModalComponents();

/**
 * The Synth-A-Modeler Designer logger function.
 *
 * Writes log to a file and post to stdout when CONFIG is Debug.
 *
 * @param message		the message to log
 */
void SAMLogger(const String& message);

/**
 * Gets a FileLogger object.
 *
 * @return				FileLogger
 */
FileLogger* getLogger();

const String getSAMLog();

/**
 * Opens help webpage in default browser.
 */
void openHelpUrl();

void openHelpUrl(const String& helpId);
void openHelpPatch(const String& helpId);
/**
 * Opens data directory in file explorer.
 */
void openDataDir();

void openFileExternal(const String& path);

void openFileNative(const String& path);

String fixPath(const String& path);

bool writeStringToFile(const String& s, const File& f);

template<class T>
T constrain(T actual, T min, T max)
{
    if(actual < min)
        return min;
    else if(actual > max)
        return max;
    else
        return actual;
}

int getCurrentTimeMillis();

void uniquifyPaths(HashMap<String, String>& paths);

void setLocale();

void cleanDataDir(const MDLFile& mdlFile);

void cleanDataDirAll();

};

}


#endif  // __MISCUTILITIES_H_64FF5DE5__
