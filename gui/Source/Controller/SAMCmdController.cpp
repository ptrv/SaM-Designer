/*
  ==============================================================================

    SAMCmdController.cpp
    Created: 4 Aug 2013 1:30:10pm
    Author:  peter

  ==============================================================================
*/
#include "../Application/CommonHeaders.h"
#include "../Models/MDLFile.h"
#include "../Models/SAMCmd.h"

#include "SAMCmdController.h"

using namespace synthamodeler;

SAMCmdController::SAMCmdController()
{
}

SAMCmdController::~SAMCmdController()
{
}

void SAMCmdController::generateDSP(MDLFile* mdl)
{
        bool r = true;
    if (StoredSettings::getInstance()->getIsExportConfirm())
        r = Alerts::confirmExport("Really export faust");

    if (! SAMCmd::isPerlAvailable())
    {
        SAM_CONSOLE("Error: ", "Perl is missing!", false);
        Alerts::missingPerl();
        return;
    }
    if (! SAMCmd::isSAMpreprocessorCmdAvailable())
    {
        SAM_CONSOLE("Error: ", "SAM-preprocessor is missing!", false);
        Alerts::missingSAMpreprocessor();
        return;
    }
    if (! SAMCmd::isSynthAModelerCmdAvailable())
    {
        SAM_CONSOLE("Error: ", "Synth-A-Modeler is missing!", false);
        Alerts::missingSAM();
        return;
    }

    if (r)
    {
        if (mdl->save(true, true) != FileBasedDocument::savedOk)
        {
            SAM_CONSOLE("Log: ", "Generate Faust code canceled!", false);
            return;
        }

        // if current MDL file is not in data dir make a temp copy in data dir
        SAMCmd* samCmd = new SAMCmd(mdl->getFilePath(), SAMCmd::FAUSTCODE);

        addJobToPool(samCmd);
    }
}

void SAMCmdController::generateBinary(MDLFile* mdl)
{
    if (mdl->getName().compare("Untitled") == 0)
    {
        SAM_CONSOLE("Error", "No mdl file", false);
        return;
    }

    if (!SAMCmd::isFaustAvailable())
    {
        SAM_CONSOLE("Error", "Missing faust executable", false);
        Alerts::missingFaust();
        return;
    }

    bool r = true;
    if (StoredSettings::getInstance()->getIsExportConfirm())
        r = Alerts::confirmExport("Really export faust");

    if (r)
    {
        if (mdl->save(true, true) != FileBasedDocument::savedOk)
        {
            SAM_CONSOLE("Error", "Generating binary canceled", false);
            return;
        }

        bool runSAM = StoredSettings::getInstance()->getRunSAMBeforeExternal();
        SAMCmd* samCmd = new SAMCmd(mdl->getFilePath(), SAMCmd::BINARY, runSAM);

        addJobToPool(samCmd);
    }

}


void SAMCmdController::addJobToPool(SAMCmd* cmdJob)
{
    if(pool.getNumJobs() == 0)
        pool.addJob(cmdJob, true);
    else
        delete cmdJob;
}