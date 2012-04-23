/*
  ==============================================================================

    ExportPanel.h
    Created: 22 Apr 2012 8:26:07pm
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

#ifndef __EXPORTPANEL_H_CA4E7DCE__
#define __EXPORTPANEL_H_CA4E7DCE__


class ExportPanel  : public DialogWindow
{
public:
	ExportPanel();
    ~ExportPanel();

    void closeButtonPressed();

    static int show();

    int returnVal;
};


#endif  // __EXPORTPANEL_H_CA4E7DCE__
