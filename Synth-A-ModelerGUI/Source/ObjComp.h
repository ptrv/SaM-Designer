/*
  ==============================================================================

    ObjComp.h
    Created: 11 Apr 2012 5:10:20pm
    Author:  Peter Vasil

  ==============================================================================
*/

#ifndef __OBJCOMP_H_F3604232__
#define __OBJCOMP_H_F3604232__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ObjController.h"

class ObjComp : public Component {
public:
	ObjComp(ObjController& objController_);
	~ObjComp();

	void paint(Graphics& g);
//	void resized();

private:
	ObjController& objController;
};


#endif  // __OBJCOMP_H_F3604232__
