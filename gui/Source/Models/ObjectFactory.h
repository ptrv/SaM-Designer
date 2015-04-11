/*
  ==============================================================================

    ObjectFactory.h
    Created: 6 May 2012 11:40:40pm
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

#ifndef __OBJECTFACTORY_H_2132166E__
#define __OBJECTFACTORY_H_2132166E__

#include "JuceHeader.h"

namespace synthamodeler
{

class ObjectFactory
{
public:
    static ValueTree createNewObjectTree(const Identifier& objType,
                                         const String& newName,
                                         int x, int y);
    static ValueTree createNewLinkObjectTree(const Identifier& linkType,
                                             const String& newName,
                                             const String& startObject,
                                             const String& endObject);
    static ValueTree createParamsTree(StringArray p);

    static ValueTree createAudioSourceTree(const String& srcName,
                                           const String& srcVal);

private:
    static ValueTree createMewMassLikeTree(const Identifier& massType,
                                           const String& newName, int x, int y,
                                           const StringPairArray& paramsStringPairs);
    
    static ValueTree createNewMassTree(const String& newName, int x, int y);
    static ValueTree createNewPortTree(const String& newName, int x, int y);
    static ValueTree createNewGroundTree(const String& newName, int x, int y);
    static ValueTree createNewResonatorsTree(const String& newName, int x, int y);

    static ValueTree createNewLinkLikeTree(const Identifier& linkType,
                                           const String& newName,
                                           const String& startObject,
                                           const String& endObject,
                                           const StringPairArray& paramsStringPairs);

    static ValueTree createNewLinkTree(const String& newName,
                                       const String& startObject,
                                       const String& endObject);
    static ValueTree createNewTouchTree(const String& newName,
                                        const String& startObject,
                                        const String& endObject);
    static ValueTree createNewPulsetouchTree(const String& newName,
                                             const String& startObject,
                                             const String& endObject);
    static ValueTree createNewPluckTree(const String& newName,
                                        const String& startObject,
                                        const String& endObject);
    static ValueTree createNewWaveguideTree(const String& newName,
                                            const String& startObject,
                                            const String& endObject);

    static ValueTree createNewTerminationTree(const String& newName, int x, int y);
    static ValueTree createNewJunctionTree(const String& newName, int x, int y);

    static ValueTree createNewAudioOutTree(const String& newName, int x, int y);
    static ValueTree createNewCommentTree(const String& newName, int x, int y);

    static ValueTree createResonatorParamsTree(StringArray p);
};
}

#endif  // __OBJECTFACTORY_H_2132166E__
