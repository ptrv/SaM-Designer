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

    enum PostLevel
    {
        ALL = 0,
        VERBOSE,
        NORMAL,
        ERROR,
        FATAL,
    };

    void init();
    
    void post(const String& textToWrite, const PostLevel pl=ALL, bool isBold=false);
//    void post(const String& textToWrite, bool isBold);
    void postLocked(const String& textToWrite, const PostLevel pl=ALL,
                    bool isBold = false);

    void toFront();
    void clear();

private:
    ScopedPointer<PostWindow> postWin;

    PostLevel currentPosLevel;
};

}

#endif  // __POSTWINDOWCONTROLLER_H_4E9C4940__
