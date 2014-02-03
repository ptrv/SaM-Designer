/*
  ==============================================================================

    MDLFileHelper.cpp
    Created: 2 Feb 2014 9:21:22pm
    Author:  peter

  ==============================================================================
 */

#include "../Application/CommonHeaders.h"
#include "../Models/MDLFile.h"
#include "../View/ContentComp.h"
#include "../View/ObjectsHolder.h"

#include "MDLHelper.h"

using namespace synthamodeler;

const String MDLHelper::getMDLInfoString(const MDLFile& mdlFile)
{
    const ValueTree& mdlRoot = mdlFile.getMDLRoot();
    
    String props;
    props << "MDL path: " << mdlRoot[Ids::mdlPath].toString() << newLine;
    props << newLine;

    int numMasses, numRes, numGrounds, numPorts, numLinks, numTouchs, numPlucks,
        numPulsetouchs, numAudioOuts, numWaveguides, numTerms, numJuncts,
        numFauscode;
    numMasses = numRes = numGrounds = numPorts = numLinks = numTouchs = numPlucks
        = numPulsetouchs = numAudioOuts = numWaveguides = numTerms = numJuncts
        = numFauscode = 0;
    if (mdlRoot.getChildWithName(Objects::masses).isValid())
    {
        ValueTree masses = mdlRoot.getChildWithName(Objects::masses);
        for (int i = 0; i < masses.getNumChildren(); ++i)
        {
            if (masses.getChild(i).getType() == Ids::mass)
            {
                ++numMasses;
            }
            else if (masses.getChild(i).getType() == Ids::ground)
            {
                ++numGrounds;
            }
            else if (masses.getChild(i).getType() == Ids::port)
            {
                ++numPorts;
            }
            else if (masses.getChild(i).getType() == Ids::resonators)
            {
                ++numRes;
            }
        }
    }
    if (mdlRoot.getChildWithName(Objects::links).isValid())
    {
        ValueTree links = mdlRoot.getChildWithName(Objects::links);
        for (int i = 0; i < links.getNumChildren(); ++i)
        {
            if (links.getChild(i).getType() == Ids::link)
            {
                ++numLinks;
            }
            else if (links.getChild(i).getType() == Ids::touch)
            {
                ++numTouchs;
            }
            else if (links.getChild(i).getType() == Ids::pluck)
            {
                ++numPlucks;
            }
            else if (links.getChild(i).getType() == Ids::pulsetouch)
            {
                ++numPulsetouchs;
            }
        }
    }
    if (mdlRoot.getChildWithName(Objects::audioobjects).isValid())
    {
        numAudioOuts = mdlRoot.getChildWithName(Objects::audioobjects).getNumChildren();
    }
    if (mdlRoot.getChildWithName(Objects::waveguides).isValid())
    {
        numWaveguides = mdlRoot.getChildWithName(Objects::waveguides).getNumChildren();
    }
    if (mdlRoot.getChildWithName(Objects::terminations).isValid())
    {
        numTerms = mdlRoot.getChildWithName(Objects::terminations).getNumChildren();
    }
    if (mdlRoot.getChildWithName(Objects::junctions).isValid())
    {
        numJuncts = mdlRoot.getChildWithName(Objects::junctions).getNumChildren();
    }
    if (mdlRoot.getChildWithName(Objects::faustcodeblock).isValid())
    {
        numFauscode = mdlRoot.getChildWithName(Objects::faustcodeblock).getNumChildren();
    }
    props << "Masses: " << numMasses << newLine;
    props << "Grounds: " << numGrounds << newLine;
    props << "Ports: " << numPorts << newLine;
    props << "Resonators: " << numRes << newLine;
    props << "Links: " << numLinks << newLine;
    props << "Touchs: " << numTouchs << newLine;
    props << "Plucks: " << numPlucks << newLine;
    props << "Pulsetouchs: " << numPulsetouchs << newLine;
    props << "AudioOuts: " << numAudioOuts << newLine;
    props << "Waveguides: " << numWaveguides << newLine;
    props << "Terminations: " << numTerms << newLine;
    props << "Junctions: " << numJuncts << newLine;
    props << "Faustcode lines: " << numFauscode << newLine;

    props << newLine;

    props << "---------------------" << newLine;

    props << newLine;

    const File& tmpFile = mdlFile.getFile();
    props << "Size: " << String(tmpFile.getSize() / 1024.0, 3) << " KB" << newLine;
    props << "Creation time: " << tmpFile.getCreationTime().formatted("%c") << newLine;
    props << "Modification time: " << tmpFile.getLastModificationTime().formatted("%c") << newLine;
    props << "On harddisk: " << tmpFile.isOnHardDisk() << newLine;

    return props;
}

ValueTree MDLHelper::getObjectWithName(const MDLFile& mdlFile, const String& objName)
{
    const ValueTree& mdlRoot = mdlFile.getMDLRoot();

    for (int i = 0; i < mdlRoot.getNumChildren(); ++i)
    {
        for (int j = 0; j < mdlRoot.getChild(i).getNumChildren(); ++j)
        {
            ValueTree ch = mdlRoot.getChild(i).getChild(j);
            if (ch[Ids::identifier].toString().compare(objName) == 0)
            {
                return ch;
            }
        }
    }
    return ValueTree::invalid;

}

bool MDLHelper::addOutputDSPVarIfNotExists(const MDLFile& mdlFile)
{
    ValueTree mdlRoot = mdlFile.getMDLRoot();
    bool outputDSPExists = false;

    ValueTree fcb = mdlRoot.getOrCreateChildWithName(Objects::faustcodeblock, nullptr);
    for (int i = 0; i < fcb.getNumChildren(); ++i)
    {
        ValueTree fc = fcb.getChild(i);
        if (fc.hasProperty(Ids::value))
        {
            String fcStr = fc[Ids::value].toString();
            if (fcStr.startsWith("outputDSP"))
            {
                outputDSPExists = true;
                break;
            }
        }
    }
    if (!outputDSPExists)
    {
        ValueTree fc(Ids::faustcode);
        fc.setProperty(Ids::value, "outputDSP=SAMlimiter:highpass(4,20.0);", nullptr);
        fcb.addChild(fc, -1, nullptr);
        return true;
    }
    return false;
}

void MDLHelper::saveMDLFileAsImage(const MDLFile& mdlFile,
                                       const ContentComp* contentComp)
{
    if (contentComp)
    {
        ObjectsHolder * const oh = contentComp->getHolderComponent();
        Image img = oh->createComponentSnapshot(oh->getObjectsBounds());

        String initFilePath;
        initFilePath << mdlFile.getFile().getParentDirectory().getFullPathName()
            << "/" << mdlFile.getFile().getFileNameWithoutExtension()
            << "_" << Time::getCurrentTime().formatted("%Y%m%d%H%M%S")
            << ".png";

        FileChooser fc(TRANS("Please select the filename for the image") + "...",
                       File::createFileWithoutCheckingPath(initFilePath),
                       "*.png;*.jpg;*.jpeg", true);

        if (fc.browseForFileToSave(true))
        {
            File f(fc.getResult());
            TemporaryFile temp(f);

            ScopedPointer <FileOutputStream> out(temp.getFile().createOutputStream());

            if (out != nullptr)
            {
                bool imgToStreamOk = false;
                if (f.hasFileExtension(".png"))
                {
                    PNGImageFormat png;
                    imgToStreamOk = png.writeImageToStream(img, *out.get());
                }
                else if (f.hasFileExtension(".jpg;.jpeg"))
                {
                    JPEGImageFormat jpeg;
                    imgToStreamOk = jpeg.writeImageToStream(img, *out.get());
                }
                out = nullptr;

                bool succeeded = false;
                if (imgToStreamOk)
                    succeeded = temp.overwriteTargetFileWithTemporary();

                if (succeeded)
                {
                    SAM_CONSOLE("MSG: Wrote file " + f.getFullPathName(), PostLevel::ALL);
                }
                else
                {
                    SAM_CONSOLE("Could not write file " + f.getFullPathName(), PostLevel::ERROR);
                }
            }
        }
    }
}

bool MDLHelper::saveMDLFileAsXml(const MDLFile& mdlFile)
{
    bool saveOk = false;
    FileChooser fc(TRANS("Select XML file to save") + "...",
                   File::getSpecialLocation(File::userHomeDirectory),
                   "*.xml");

    if (fc.browseForFileToSave(true))
    {
        File xmlFile(fc.getResult());
        String mdlXmlStr = mdlFile.toString();

        saveOk = Utils::writeStringToFile(mdlXmlStr, xmlFile);
    }
    return saveOk;

}