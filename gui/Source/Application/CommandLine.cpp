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
bool checkFilesForIO(const String& path1, const String& path2, File& f1, File& f2,
                     const String& outExt, bool sameDir)
{
    if(File::isAbsolutePath(path1))
        f1 = path1;
    else
        f1 = File::getCurrentWorkingDirectory().getChildFile(path1);

    if(! f1.existsAsFile())
    {
        std::cout << "Input file: " << f1.getFullPathName().toUTF8().getAddress();
        std::cout << " does not exist" << std::endl;
        return false;
    }
    if(sameDir)
    {
        f2 = f1.getParentDirectory().getFullPathName() + "/"
            + f1.getFileNameWithoutExtension() + outExt;
    }
    else
    {
        if(File::isAbsolutePath(path2))
            f2 = path2;
        else
            f2 = File::getCurrentWorkingDirectory().getChildFile(path2);
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
    File outFile;
    bool createOutputInSameDir = false;
    String outPath;
    if(args.size() == 2)
    {
        createOutputInSameDir = true;
        outPath = String::empty;
    }
    else
    {
        outPath = args[2];
    }
    if(! checkFilesForIO(args[1], outPath, in, outFile, "*.dsp", createOutputInSameDir))
    {
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
    if(File::isAbsolutePath(args[1]))
        in = args[1];
    else
        in = File::getCurrentWorkingDirectory().getChildFile(args[1]);

    if(! in.existsAsFile())
    {
        std::cout << "Input file: " << in.getFullPathName().toUTF8().getAddress();
        std::cout << " does not exist" << std::endl;
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

    std::cout << "Not implemented yet!" << std::endl;
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
    if (matchArgument (args[0], "print-xml"))         return printMdlXml (args);
    if (matchArgument (args[0], "list-exporters"))    return listExporters(false);
    if (matchArgument (args[0], "list-exportersd"))   return listExporters(true);
    if (matchArgument (args[0], "binary"))            return generateBinary(args);

    return commandLineNotPerformed;
}
