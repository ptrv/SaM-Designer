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

#include "../Controller/ObjController.h"
/**
 * Namespace for object creation functions.
 */
namespace ObjectFactory
{

	/**
	 * Creates a new object value tree.
	 *
	 * @param objType			the object type id (i.e. Ids::mass)
	 * @param x					initial x position
	 * @param y					initial y position
	 * @return					a new ValueTree with default values
	 */
	ValueTree createNewObjectTree(const Identifier& objType, 
                                  const String& newName,
                                  int x, int y);
    ValueTree createNewLinkObjectTree(const Identifier& linkType,
                                      const String& newName,
                                      const String& startObject,
                                      const String& endObject);
    ValueTree createParamsTree(StringArray p);
    ValueTree createLabelsTree(StringArray p);
}


#endif  // __OBJECTFACTORY_H_2132166E__
