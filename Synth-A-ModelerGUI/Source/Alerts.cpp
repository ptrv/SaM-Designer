/*
  ==============================================================================

    Alerts.cpp
    Created: 12 Apr 2012 2:16:00am
    Author:  peter

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Alerts.h"

void Alerts::missingFaust()
{
	String title = "Missing faust executable";
	String msg;
	AlertWindow::showMessageBox(AlertWindow::WarningIcon, title, msg, "OK");
}

void Alerts::missingPerl()
{
	String title = "Missing perl executable";
	String msg;
	AlertWindow::showMessageBox(AlertWindow::WarningIcon, title, msg, "OK");

}
void Alerts::missingSAM()
{
	String title = "Missing Synth-A-Modeler compiler";
	String msg;
	AlertWindow::showMessageBox(AlertWindow::WarningIcon, title, msg, "OK");

}

