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

/**
 * Formats parameter value to have decmal point and zero.
 *
 * @param paramVal		the parameter value to format
 * @return				return a formatted parameter value
 */
String fixParameterValueIfNeeded(const String& paramVal);

String getGainForSourceId(ValueTree& sources, const String& sourceId);
void setGainForSourceId(ValueTree& sources, const String& sourceId,
                        const String& gainVal, UndoManager* undoManager);

BaseObjectComponent* getBaseObjectFromSource(ObjController* objController, ValueTree& source);

const Identifier& getObjectGroup(const Identifier& ident);

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

bool containsStringInValueTree(ValueTree valTree,
                               const String& searchStr,
                               bool isRoot);

extern const Array<Identifier> resonatorParamIds;

extern const Array<Identifier> allObjectIds;

int getCurrentTimeMillis();

/**
 * Checks wether the Synth-A-Modeler script is available.
 *
 * @return			true if Synth-A-Modeler is found
 */
bool isSynthAModelerCmdAvailable();
/**
 * Checks wether the SAM-preprocessor script is available.
 *
 * @return			true if SAM-preprocessor is found
 */
bool isSAMpreprocessorCmdAvailable();
/**
 * Checks wether a specifdied command is available.
 *
 * @param cmdStr	command string
 * @return			true if specifdied command is available
 */
bool isCmdAvailable(const String& cmdStr);
/**
 * Checks wether perl is available.
 *
 * @return			true if the perl executable is found
 */
bool isPerlAvailable();
/**
 * Checks wether faust executable is available.
 *
 * @return			true if faust executable is found
 */
bool isFaustAvailable();

const String getSAMLog();

};

}


#endif  // __MISCUTILITIES_H_64FF5DE5__
