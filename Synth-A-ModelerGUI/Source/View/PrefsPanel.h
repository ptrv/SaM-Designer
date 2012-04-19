/*
  ==============================================================================

    PrefsPanel.h
    Created: 19 Apr 2012 2:25:09pm
    Author:  peter

  ==============================================================================
*/

#ifndef __PREFSPANEL_H_87C5CC8A__
#define __PREFSPANEL_H_87C5CC8A__

class PrefsPanel  : public DialogWindow
{
public:
    PrefsPanel();
    ~PrefsPanel();

    void closeButtonPressed();

    static void show();
};



#endif  // __PREFSPANEL_H_87C5CC8A__
