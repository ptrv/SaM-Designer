/*
  ==============================================================================

    CommonHeaders.h

    Created: 22 Jan 2012 10:50:41pm
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

#ifndef __COMMONHEADERS_H_CA475C67__
#define __COMMONHEADERS_H_CA475C67__

/**
 * All common includes.
 */

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../Utilities/StoredSettings.h"
#include "CommandIDs.h"
#include "../Models/ObjectIDs.h"
#include "../Utilities/Alerts.h"
#include "../Utilities/MiscUtilities.h"
#include "../Utilities/ResourceLoader.h"
#include "Application.h"

extern ScopedPointer<ApplicationCommandManager> commandManager;

#ifdef DEBUG
#define SAM_LOG(message) Utils::SAMLogger(message)
#else
#define SAM_LOG(message)
#endif

#define SAM_CONSOLE(title, message) SynthAModelerApplication::getApp()->writeToDebugConsole(title, message)

#endif  // __COMMONHEADERS_H_CA475C67__
