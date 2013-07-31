/*
  ==============================================================================

    PostWindow.h
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

#ifndef __POSTWINDOW_H_3A3F156F__
#define __POSTWINDOW_H_3A3F156F__

namespace synthamodeler
{
class TextConsole;

/**
 * A window showing output of Synth-A-Modeler script, faust and other
 * application activity.
 */
class PostWindow : public DocumentWindow,
					public ApplicationCommandTarget
{
public:
	PostWindow();
	~PostWindow();

	void closeButtonPressed();

	/**
	 * Toggels visibility of OutputWindow.
	 */
	void toggleDebugWindow();

	/**
	 * prints a predefined text as header
	 */
	void printHeader();

	/**
	 * Clears the text in OutputWindow.
	 */
	void clear();

	/**
	 * Inserts text into the TextConsole.
	 *
	 * @param compilerText	string text to insert.
	 */
	void addText(const String& compilerText, bool isBold=false);

	/**
	 * Makes window visible.
	 */
	void makeVisible();

	/**
	 * Hides winidow.
	 */
	void makeHide();

	/**
	 * prints a predefined welcome message to OutputWindow when application starts.
	 */
	void printWelcomeMessage();

	/**
	 * Inserts a new line.
	 */
	void addNewLine();

    void setTextColour(const Colour& textColour);

	//==========================================================================
    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);
	//==========================================================================

private:
	TextConsole* console;
};
}

#endif  // __POSTWINDOW_H_3A3F156F__
