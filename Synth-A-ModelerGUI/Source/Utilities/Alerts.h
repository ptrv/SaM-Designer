/*
  ==============================================================================

    Alerts.h
    Created: 12 Apr 2012 2:16:00am
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

#ifndef __ALERTS_H_2AC9A37F__
#define __ALERTS_H_2AC9A37F__

/**
 * Static helper class for various alerts.
 */
class Alerts
{
private:
	Alerts();
	~Alerts();
public:
	static void missingPerl();
	static void missingSAM();
	static void missingFaust();
	static void wrongFileType();
	static bool confirmExport(const String& text);
    static void nameAlreadyExists();
private:
	static void missingAlert(const String& title, const String& msg);

};


#endif  // __ALERTS_H_2AC9A37F__
