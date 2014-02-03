/*
  ==============================================================================

    MDLController.cpp
    Created: 12 Apr 2012 11:49:49pm
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

#include "../Application/CommonHeaders.h"
#include "../Application/MainWindow.h"
#include "../Models/MDLFile.h"
#include "../View/ObjectsHolder.h"
#include "../View/ContentComp.h"
#include "../Controller/SAMCmdController.h"
#include "../Utilities/MDLHelper.h"

#include "MDLController.h"

using namespace synthamodeler;

const char* MDLController::fileTypesToDelete[] = {".dsp", ".mdx", ".dsp.xml", ".cpp"};

MDLController::MDLController(MainAppWindow& mainAppWindow_)
: mainAppWindow(mainAppWindow_),
  currentMdl(nullptr)
{
	samCmdCtrl = new SAMCmdController();
}

MDLController::~MDLController()
{
	currentMdl = nullptr;
	samCmdCtrl = nullptr;
}

void MDLController::newFile()
{
	FileBasedDocument::SaveResult sr = currentMdl->saveIfNeededAndUserAgrees();
	if (sr != FileBasedDocument::userCancelledSave)
	{
		currentMdl->newMDL();
	}
}
void MDLController::open()
{
	FileBasedDocument::SaveResult sr = currentMdl->saveIfNeededAndUserAgrees();
	if (sr != FileBasedDocument::userCancelledSave)
	{
		bool loadOk = currentMdl->loadFromUserSpecifiedFile(true);

		if(loadOk)
		{
			StoredSettings::getInstance()->recentFiles.addFile(currentMdl->getFile());
		}
//		else
//		{
//			DBG("Something went wrong loading the mdl file.");
//		}
	}
}

void MDLController::openFromFile(const File& mdlFile)
{
	FileBasedDocument::SaveResult sr = currentMdl->saveIfNeededAndUserAgrees();
	if (sr != FileBasedDocument::userCancelledSave)
	{
		currentMdl->loadFrom(mdlFile, true);
	}
}
bool MDLController::save()
{
    if (currentMdl->checkIfChecksumChanged())
    {
        int res = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon,
                                                  TRANS("MDL file changed outside!"),
                                                  "",
                                                  TRANS("Load from disk"),
                                                  TRANS("Overwrite"),
                                                  TRANS("Cancel"));
        switch (res)
        {
        case 0:
            return false;
        case 1:
            currentMdl->close();
            openFromFile(currentMdl->getFile());
            mainAppWindow.getMDLFileContentComponent()->reloadMDLFile(getMDLFile());
            getUndoManager().clearUndoHistory();
            return false;
        case 2:
            return currentMdl->save(false, true) == FileBasedDocument::savedOk;
        }
    }
    else
    {
        bool saveOk = currentMdl->save(true, true) == FileBasedDocument::savedOk;
        if (!saveOk)
        {
            SAM_LOG("Something went wrong saving the mdl file.");
        }
        return saveOk;
    }
}
void MDLController::saveAs()
{
	if(currentMdl->saveAsInteractive(true) != FileBasedDocument::savedOk)
	{
		SAM_LOG("Something went wrong saving the mdl file.");
	}
//    else
//    {
//        close();
//        openFromFile(currentMdl->getFile());
//    }
}

void MDLController::saveAsImage()
{
    MDLHelper::saveMDLFileAsImage(*currentMdl.get(),
                                  mainAppWindow.getMDLFileContentComponent());
}
void MDLController::close()
{
	FileBasedDocument::SaveResult sr = currentMdl->saveIfNeededAndUserAgrees();
	if(sr != FileBasedDocument::userCancelledSave)
	{
		currentMdl->close();
	}
}

bool MDLController::saveAsXml()
{
    return MDLHelper::saveMDLFileAsXml(*currentMdl.get());
}

void MDLController::generateFaust()
{
    samCmdCtrl->generateDSP(currentMdl);
}

void MDLController::generateExternal()
{
    samCmdCtrl->generateBinary(currentMdl);
}

const String MDLController::getMDLName()
{
	return currentMdl->getName();
}

UndoManager& MDLController::getUndoManager()
{
    return currentMdl->getUndoMgr();
}

bool MDLController::perform (UndoableAction* const action, const String& actionName)
{
    getUndoManager().beginNewTransaction(actionName);
    return getUndoManager().perform(action, actionName);
}

ValueTree MDLController::getMDLTree()
{
	if(currentMdl == nullptr)
		return ValueTree::invalid;

    return currentMdl->getMDLRoot();
}

bool MDLController::mdlCheckAndSaveIfNeeded()
{
	FileBasedDocument::SaveResult sr = currentMdl->saveIfNeededAndUserAgrees();
	if(sr == FileBasedDocument::userCancelledSave)
		return false;
	else
		return true;

}

MDLFile* MDLController::getMDLFile() const
{
	return currentMdl.get();
}

void MDLController::setMDLFile(MDLFile* mdlFile)
{
	currentMdl = mdlFile;
	if(currentMdl != nullptr)
	{
		StoredSettings::getInstance()->recentFiles.addFile(currentMdl->getFile());
		StoredSettings::getInstance()->flush();
	}
}

void MDLController::changed()
{
    currentMdl->changed();
    mainAppWindow.updateTitle();
}

ObjectsHolder* MDLController::getHolderComponent()
{
    return mainAppWindow.getHolderComponent();
}

void MDLController::cleanDataDir()
{
    Utils::cleanDataDir(*currentMdl.get());
}

void MDLController::cleanDataDirAll()
{
    Utils::cleanDataDirAll();
}

bool MDLController::checkIfMdlCanchedOutside()
{
    return false;
//    if (currentMdl->checkIfChecksumChanged())
//    {
//        int res = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon,
//                                                  "MDL file changed outside!",
//                                                  "",
//                                                  "Load from disk",
//                                                  "Overwrite",
//                                                  "Cancel");
//        switch (res)
//        {
//        case 0:
//            break;
//        case 1:
//            currentMdl->close();
//            openFromFile(currentMdl->getFile());
//            mainAppWindow.getMDLFileContentComponent()->reloadMDLFile(getMDLFile());
//            getUndoManager()->clearUndoHistory();
//            break;
//        case 2:
//            currentMdl->save(false, true);
//            return
//        }
//    }
}
