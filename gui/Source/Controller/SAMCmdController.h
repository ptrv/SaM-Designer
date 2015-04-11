/*
  ==============================================================================

    SAMCmdController.h
    Created: 4 Aug 2013 1:30:10pm
    Author:  peter

  ==============================================================================
*/

#ifndef __SAMCMDCONTROLLER_H_DE7E45EB__
#define __SAMCMDCONTROLLER_H_DE7E45EB__


#include "JuceHeader.h"

namespace synthamodeler
{
class MDLFile;
class SAMCmd;

class SAMCmdController
{
public:
    SAMCmdController();
    ~SAMCmdController();

    void generateDSP(MDLFile* mdl);
    void generateBinary(MDLFile* mdl);

private:
    void addJobToPool(SAMCmd* cmdJob);
    ThreadPool pool;
};

}


#endif  // __SAMCMDCONTROLLER_H_DE7E45EB__
