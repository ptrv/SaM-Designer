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


namespace ObjectFactory
{

	ValueTree createNewObjectTree(const Identifier& objType, int x, int y);

}


#endif  // __OBJECTFACTORY_H_2132166E__
