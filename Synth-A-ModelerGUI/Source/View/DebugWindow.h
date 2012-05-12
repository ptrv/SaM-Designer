/*
  ==============================================================================

    DebugWindow.h
    Created: 11 Apr 2012 3:18:15pm
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

#ifndef __DEBUGWINDOW_H_3A3F156F__
#define __DEBUGWINDOW_H_3A3F156F__

class TextConsole;
class DebugWindow : public DocumentWindow,
					public ApplicationCommandTarget
{
public:
	DebugWindow();
	~DebugWindow();

	void closeButtonPressed();

	void toggleDebugWindow();

	void printHeader();

	void clear();

	void addText(const String& compilerText);

	void makeVisible();
	void makeHide();

	void printWelcomeMessage();
	void addNewLine();

    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);

private:
	TextConsole* console;
};


#endif  // __DEBUGWINDOW_H_3A3F156F__
