/*
  ==============================================================================

    ObjectIDs.h
    Created: 3 May 2012 9:15:13pm
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

#ifndef __OBJECTIDS_H_C7C4C1BC__
#define __OBJECTIDS_H_C7C4C1BC__


/**
 * Namespace for Ids used for the ValueTrees.
 */
namespace Ids
{
    #define DECLARE_ID(name)      const Identifier name (#name)

	DECLARE_ID (mdlName);
	DECLARE_ID (mdlPath);
	DECLARE_ID (mass);
	DECLARE_ID (port);
	DECLARE_ID (ground);
	DECLARE_ID (resonator);
	DECLARE_ID (link);
	DECLARE_ID (pluck);
	DECLARE_ID (touch);
	DECLARE_ID (label);
	DECLARE_ID (audioout);
	DECLARE_ID (waveguide);
	DECLARE_ID (termination);
	DECLARE_ID (junction);
	DECLARE_ID (parameter);
    DECLARE_ID (parameters);
    DECLARE_ID (type);
    DECLARE_ID (identifier);
    DECLARE_ID (kg);
    DECLARE_ID (initPos);
    DECLARE_ID (initVel);
    DECLARE_ID (labels);
    DECLARE_ID (posX);
    DECLARE_ID (posY);
    DECLARE_ID (stiffness);
    DECLARE_ID (damping);
    DECLARE_ID (offset);
    DECLARE_ID (startVertex);
    DECLARE_ID (endVertex);
    DECLARE_ID (minDisplaceDiff);
    DECLARE_ID (waveImpedance);
    DECLARE_ID (delay);
    DECLARE_ID (objLeft);
    DECLARE_ID (objRight);
    DECLARE_ID (termFilter);
    DECLARE_ID (initDisplace);
    DECLARE_ID (faustCode);
    DECLARE_ID (audiosource);
    DECLARE_ID (sources);
    DECLARE_ID (optional);
    DECLARE_ID (value);
    DECLARE_ID (variable);
    DECLARE_ID (comment);

    const Identifier class_ ("class");

    #undef DECLARE_ID
}

namespace Objects
{
	#define DECLARE_ID(name)      const Identifier name (#name)

	DECLARE_ID(MDLROOT);
	DECLARE_ID(masses);
	DECLARE_ID(links);
    DECLARE_ID(variables);
	DECLARE_ID(audioobjects);
	DECLARE_ID(waveguides);
	DECLARE_ID(terminations);
	DECLARE_ID(junctions);
	DECLARE_ID(invalid);
    DECLARE_ID(comments);

	#undef DECLARE_ID
};



#endif  // __OBJECTIDS_H_C7C4C1BC__
