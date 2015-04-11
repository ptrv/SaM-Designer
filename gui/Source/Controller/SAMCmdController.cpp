/*
  ==============================================================================

    SAMCmdController.cpp
    Created: 4 Aug 2013 1:30:10pm
    Author:  peter

  ==============================================================================
*/
#include "SAMCmdController.h"

#include "Application/CommonHeaders.h"

#include "Models/MDLFile.h"
#include "Models/SAMCmd.h"

#include "Utilities/CmdHelper.h"


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

    if (! CmdHelper::isPerlAvailable())
    {
        SAM_CONSOLE("Perl is missing!", PostLevel::ERROR);
        Alerts::missingPerl();
        return;
    }
    if (! CmdHelper::isSAMpreprocessorCmdAvailable())
    {
        SAM_CONSOLE("SAM-preprocessor is missing!", PostLevel::ERROR);
        Alerts::missingSAMpreprocessor();
        return;
    }
    if (! CmdHelper::isSynthAModelerCmdAvailable())
    {
        SAM_CONSOLE("Synth-A-Modeler is missing!", PostLevel::ERROR);
        Alerts::missingSAM();
        return;
    }

    if (r)
    {
        if (mdl->save(true, true) != FileBasedDocument::savedOk)
        {
            SAM_CONSOLE("Generate Faust code canceled!", PostLevel::ALL);
            return;
        }

        SAMCmd* samCmd = new SAMCmd();
        samCmd->setupJob(mdl->getFilePath(), SAMCmd::FAUSTCODE);

        addJobToPool(samCmd);
    }
}

void SAMCmdController::generateBinary(MDLFile* mdl)
{
    if (mdl->getName().compare("Untitled") == 0)
    {
        SAM_CONSOLE("No mdl file", PostLevel::ERROR);
        return;
    }

    if (!CmdHelper::isFaustAvailable())
    {
        SAM_CONSOLE("Missing faust executable", PostLevel::ERROR);
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
            SAM_CONSOLE("Generating binary canceled", PostLevel::ERROR);
            return;
        }

        SAMCmd* samCmd = new SAMCmd();
        samCmd->setupJob(mdl->getFilePath(), SAMCmd::BINARY,
                         StoredSettings::getInstance()->getRunSAMBeforeExternal());

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
