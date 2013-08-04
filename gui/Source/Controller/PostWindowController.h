/*
  ==============================================================================

    PostWindowController.h
    Created: 2 Aug 2013 11:17:41pm
    Author:  peter

  ==============================================================================
*/

#ifndef __POSTWINDOWCONTROLLER_H_4E9C4940__
#define __POSTWINDOWCONTROLLER_H_4E9C4940__

namespace synthamodeler
{
class PostWindow;

class PostWindowController
{
public:
    PostWindowController();
    ~PostWindowController();

    void init();
    
    void post(const String& title, const String& textToWrite, bool isBold);
    void post(const String& textToWrite, bool isBold);
    void postLocked(const String& title, const String& textToWrite, bool isBold);

    void toFront();
    void clear();

private:
    ScopedPointer<PostWindow> postWin;
};

}

#endif  // __POSTWINDOWCONTROLLER_H_4E9C4940__
