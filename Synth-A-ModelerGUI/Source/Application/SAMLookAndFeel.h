/*
  ==============================================================================

    SAMLookAndFeel.h
    Created: 18 May 2012 7:55:55pm
    Author:  peter

  ==============================================================================
*/

#ifndef __SAMLOOKANDFEEL_H_F27BC606__
#define __SAMLOOKANDFEEL_H_F27BC606__

/**
 * This class is the theme fro the application's widgets (buttons, text, etc...)
 */
class SAMLookAndFeel : public OldSchoolLookAndFeel {
public:
	SAMLookAndFeel();
	~SAMLookAndFeel();

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SAMLookAndFeel);
};


#endif  // __SAMLOOKANDFEEL_H_F27BC606__
