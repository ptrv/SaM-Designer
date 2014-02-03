/*
  ==============================================================================

    MDLFileHelper.h
    Created: 2 Feb 2014 9:21:22pm
    Author:  peter

  ==============================================================================
*/

#ifndef MDLHELPER_H_INCLUDED
#define MDLHELPER_H_INCLUDED

namespace synthamodeler
{

class MDLFile;
class ContentComp;

class MDLHelper
{
public:

    static const String getMDLInfoString(const MDLFile& mdlFile);

    static ValueTree getObjectWithName(const MDLFile& mdlFile,
                                       const String& objName);

    static bool addOutputDSPVarIfNotExists(const MDLFile& mdlFile);

    static void saveMDLFileAsImage(const MDLFile& mdlFile,
                                   const ContentComp * const contentComp);
    static bool saveMDLFileAsXml(const MDLFile& mdlFile);
};

}

#endif  // MDLHELPER_H_INCLUDED
