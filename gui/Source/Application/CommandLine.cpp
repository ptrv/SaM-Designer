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
#include "../Controller/MDLController.h"

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
#if BUILTIN_SAM_COMPILER
        << " Synth-A-Modeler --compile-builtin /path/to/mdl_file /path/to/dsp_file" << std::endl
        << "    Compiles a mdl_file to a dsp_file." << std::endl
        << std::endl
#endif
        << " Synth-A-Modeler --binary exporter_name mdl_file" << std::endl
        << "    Generates a binary." << std::endl
        << std::endl
        << " Synth-A-Modeler --list-exporters" << std::endl
        << "    Lists all available exporter commands." << std::endl
        << std::endl
        << " Synth-A-Modeler --list-exportersd" << std::endl
        << "    Lists all available exporter commands with detail." << std::endl
        << std::endl
        << " Synth-A-Modeler --print-xml /path/to/mdl_file" << std::endl
        << "    Prints xml structure of mdl_file to stdout." << std::endl
        << std::endl
        << " Synth-A-Modeler --clean" << std::endl
        << "    Cleans DATA_DIR." << std::endl
        << std::endl
        << " Synth-A-Modeler --version" << std::endl
        << "    Prints version information." << std::endl
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

#if BUILTIN_SAM_COMPILER
int compileMdlToDsp (const StringArray& args, bool useBuiltinCompiler)
#else
int compileMdlToDsp (const StringArray& args)
#endif
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

#if BUILTIN_SAM_COMPILER
    if(useBuiltinCompiler)
    {
        ScopedPointer<synthamodeler::MDLFile> mdl;
        mdl = new MDLFile(in);

        String dsp = synthamodeler::SAMCompiler::compile(mdl->mdlRoot);

        if (synthamodeler::Utils::writeStringToFile(dsp, outFile))
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
    else
#endif
    {
        SAMCmd cmd;
        String result = cmd.generateFaustCode(in.getFullPathName(),
                                              outFile.getFullPathName(),
                                              false);

        std::cout << "\n" << result << std::endl;
        return 0;
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
        std::cout << "    " << pf.getAllKeys()[i];
        if(showDetails)
        {
            std::cout << ": " << std::endl;
            std::cout << "    " << pf.getAllValues()[i] << std::endl << std::endl;
        }
        else
        {
            std::cout << std::endl;
        }
    }

    return 0;
}

int cleanDataDir()
{
    StringArray filePathsToDelete;
    for (int i = 0; i < 4; ++i)
    {
        DirectoryIterator iter(StoredSettings::getInstance()->getDataDir(),
                               false, "*"+String(MDLController::fileTypesToDelete[i]));
        while (iter.next())
        {
            filePathsToDelete.add(iter.getFile().getFullPathName());
        }
    }
    String outStrOk;
    String outStrError;
    for (int j = 0; j < filePathsToDelete.size(); ++j)
    {
        File f(filePathsToDelete[j]);
        if(! f.existsAsFile())
            continue;
        if (f.moveToTrash())
            outStrOk << filePathsToDelete[j] << "\n";
        else
            outStrError << filePathsToDelete[j] << "\n";
    }
    if(outStrOk.isEmpty() && outStrError.isEmpty())
    {
        std::cout << "\nNo files were deleted." << std::endl;
        return 0;
    }
    if(outStrOk.isNotEmpty())
        std::cout << "\nDelete OK:\n" << outStrOk << std::endl;
    if(outStrError.isNotEmpty())
        std::cout << "\nError: Could not delete\n" << outStrError << std::endl;
    return 0;
}

int printDataDir()
{
    std::cout << "\nDATA_DIR: " << StoredSettings::getInstance()->getDataDir();
    std::cout << std::endl << std::endl;
    return 0;
}

String columnizeText(const String& text, const String& prependText, int textLength)
{
    String resText;
    StringArray textLines;
    StringArray textWords;
    textWords.addTokens(text, true);
    textWords.trim();
    String tmp = "";
    for (int i = 0; i < textWords.size(); ++i)
    {
        if(tmp.length()+textWords[i].length() > textLength)
        {
            textLines.add(tmp);
            tmp = "";
            tmp << textWords[i];
        }
        else
        {
            if(i != 0)
                tmp << " ";
            tmp << textWords[i];
        }
    }
    if(tmp.isNotEmpty())
        textLines.add(tmp);

    for (int i = 0; i < textLines.size(); ++i)
    {
        resText << prependText << textLines[i] << "\n";
    }
    return resText;
}
int printVersion()
{
    std::cout << "Synth-A-Modeler!" << std::endl
        << std::endl
        << "   Version: " << JUCEApplication::getInstance()->getApplicationVersion()
        << " (Build: " + String(__DATE__) << " " << String(__TIME__) + ")"
        << ", " + SystemStats::getJUCEVersion() << std::endl
        << std::endl
        << columnizeText(BinaryData::about_txt, "   ", 72)
        << std::endl;
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

    if (matchArgument(args[0], "help"))              return showHelp();
#if BUILTIN_SAM_COMPILER
    if (matchArgument(args[0], "compile"))           return compileMdlToDsp(args, false);
    if (matchArgument(args[0], "compile-builtin"))   return compileMdlToDsp(args, true);
#else
    if (matchArgument(args[0], "compile"))           return compileMdlToDsp(args);
#endif
    if (matchArgument(args[0], "binary"))            return generateBinary(args);
    if (matchArgument(args[0], "list-exporters"))    return listExporters(false);
    if (matchArgument(args[0], "list-exportersd"))   return listExporters(true);
    if (matchArgument(args[0], "print-xml"))         return printMdlXml(args);
    if (matchArgument(args[0], "clean"))             return cleanDataDir();
    if (matchArgument(args[0], "data-dir"))          return printDataDir();
    if (matchArgument(args[0], "version"))           return printVersion();

    return commandLineNotPerformed;
}
