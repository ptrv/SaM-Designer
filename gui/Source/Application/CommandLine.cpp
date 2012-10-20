/*
  ==============================================================================

    CommandLine.cpp
    Created: 19 Oct 2012 6:13:52pm
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
#include "../Models/SAMCompiler.h"
#include "../Models/MDLFile.h"
#include "../Models/SAMCmd.h"

#include "CommandLine.h"

using namespace synthamodeler;

namespace
{
void hideDockIcon()
{
#if JUCE_MAC
    Process::setDockIconVisible(false);
#endif
}

int showHelp()
{
    hideDockIcon();

    std::cout << "Synth-A-Modeler!" << std::endl
        << std::endl
        << "Usage: " << std::endl
        << std::endl
        << " Synth-A-Modeler --compile /path/to/mdl_file /path/to/dsp_file" << std::endl
        << "    Compiles a mdl_file to a dsp_file." << std::endl
        << std::endl
        << " Synth-A-Modeler --print-xml /path/to/mdl_file" << std::endl
        << "    Prints xml structure of mdl_file to stdout." << std::endl
        << std::endl
        << " Synth-A-Modeler --list-exporters" << std::endl
        << "    Lists all available exporter commands." << std::endl
        << std::endl
        << " Synth-A-Modeler --list-exportersd" << std::endl
        << "    Lists all available exporter commands with detail." << std::endl
        << std::endl
        << " Synth-A-Modeler --binary exporter_name mdl_file" << std::endl
        << "    Generates a binary." << std::endl
        << std::endl;

    return 0;
}

//==============================================================================
bool checkFile(const String& path, File& f, bool checkExistance)
{
    if(File::isAbsolutePath(path))
        f = path;
    else
        f = File::getCurrentWorkingDirectory().getChildFile(path);

    if(checkExistance)
    {
        if(! f.existsAsFile())
        {
            std::cout << "File: " << f.getFullPathName().toUTF8().getAddress();
            std::cout << " does not exist" << std::endl;
            return false;
        }
    }
    return true;
}

int compileMdlToDsp (const StringArray& args)
{
    hideDockIcon();
    
    if(args.size() != 2 && args.size() != 3)
    {
        return showHelp();
    }

    File in;
    if(! checkFile(args[1], in, true))
        return 1;

    File outFile;
    if(args.size() == 2)
    {
        outFile = in.getParentDirectory().getFullPathName() + "/"
            + in.getFileNameWithoutExtension() + ".dsp";
    }
    else
    {
        if(! checkFile(args[2], outFile, false))
            return 1;
    }

    ScopedPointer<synthamodeler::MDLFile> mdl;
    mdl = new MDLFile(in);
    
    String dsp = synthamodeler::SAMCompiler::compile(mdl->mdlRoot);
    
    if(synthamodeler::Utils::writeStringToFile(dsp, outFile))
    {
        std::cout << "\nSuccessfully created " << outFile.getFullPathName() << std::endl;
        return 0;
    }
    else
    {
        std::cout << "\nCreating " << outFile.getFullPathName() << " failed!" << std::endl;
        return 1;
    }
}

int printMdlXml(const StringArray& args)
{
    hideDockIcon();
    
    if(args.size() != 2)
    {
        return showHelp();
    }

    File in;
    if(! checkFile(args[1], in, true))
    {
        return 1;
    }

    ScopedPointer<synthamodeler::MDLFile> mdl;
    mdl = new MDLFile(in);
    std::cout << "\n" << mdl->toString() << std::endl;

    return 0;
}

int generateBinary(const StringArray& args)
{
    hideDockIcon();

    if(args.size() != 3)
    {
        showHelp();
    }

    String exporter = args[1];
    String mdlPath = args[2];
    File mdlFile;
    if(! checkFile(mdlPath, mdlFile, true))
        return 1;

    ScopedPointer<synthamodeler::MDLFile> mdl;
    mdl = new MDLFile(mdlFile);

    StringPairArray spa = StoredSettings::getInstance()->getExporters().getAllProperties();
    String exporterCmd = spa.getValue(exporter, String::empty);
    if(exporterCmd == String::empty)
    {
        std::cout << "There is no such exporter: " << exporter << std::endl;
        return 1;
    }
    SAMCmd cmd;

    std::cout << cmd.generateExternal(mdl->getFilePath(), exporterCmd, false) << std::endl;
    return 0;
}

int listExporters(bool showDetails)
{
    hideDockIcon();
    
    StringPairArray pf = StoredSettings::getInstance()->getExporters().getAllProperties();

    std::cout << "Synth-A-Modeler!" << std::endl << std::endl;
    std::cout << " Available exporters:" << std::endl << std::endl;
    for (int i = 0; i < pf.size(); ++i)
    {
        std::cout << "    " << pf.getAllKeys()[i];;
        if(showDetails)
        {
            std::cout << ": " << std::endl;
            std::cout << pf.getAllValues()[i] << std::endl << std::endl;
        }
        else
        {
            std::cout << std::endl;
        }
    }

    return 0;
}

bool matchArgument(const String& arg, const String& possible)
{
    return arg == possible
        || arg == "-" + possible
        || arg == "--" + possible;
}

}
//==============================================================================
int synthamodeler::performCommandLine (const String& commandLine)
{
    StringArray args;
    args.addTokens (commandLine, true);
    args.trim();

    if (matchArgument (args[0], "help"))              return showHelp();
    if (matchArgument (args[0], "compile"))           return compileMdlToDsp (args);
    if (matchArgument (args[0], "binary"))            return generateBinary(args);
    if (matchArgument (args[0], "list-exporters"))    return listExporters(false);
    if (matchArgument (args[0], "list-exportersd"))   return listExporters(true);
    if (matchArgument (args[0], "print-xml"))         return printMdlXml (args);

    return commandLineNotPerformed;
}
