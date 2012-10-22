/*
  ==============================================================================

  Synth-A-Modeler compiler (c++ version)
  Author: Peter Vasil (code), Edgar Berdahl (method)

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

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../gui/Source/Models/SAMCompiler.h"


int showHelp()
{
    std::cout << "Synth-A-Modeler!" << std::endl
        << std::endl
        << "Usage: " << std::endl
        << std::endl
        << " Synth-A-Modeler /path/to/inputfile.mdlx"
        << " /path/to/outputfile.dsp" << std::endl
        << std::endl;

    return 0;
}

String generateDspString(const XmlElement& xml)
{
    ValueTree mdlRoot = ValueTree::fromXml(xml);

    return synthamodeler::SAMCompiler::compile(mdlRoot);
}

bool writeStringToFile(const String& s, const String& p)
{
    File outFile;
    if(File::isAbsolutePath(p))
        outFile = p;
    else
        outFile = File::getCurrentWorkingDirectory().getChildFile(p);
    
    TemporaryFile temp(outFile);

    ScopedPointer <FileOutputStream> out(temp.getFile().createOutputStream());

    if (out != nullptr)
    {
        out->write(s.toUTF8(), s.getNumBytesAsUTF8());
        out = nullptr; // (deletes the stream)

        bool succeeded = temp.overwriteTargetFileWithTemporary();
        return succeeded;
    }
    else
    {
        return false;
    }
}
//==============================================================================
int main (int argc, char* argv[])
{
    StringArray args;
    for (int i = 1; i < argc; ++i)
    {
        args.addTokens(argv[i], true);
        args[i];
    }
    args.trim();
    if(args.size() != 2)
    {
        return showHelp();
    }

    File in;
    if(File::isAbsolutePath(args[0]))
        in = args[0];
    else
        in = File::getCurrentWorkingDirectory().getChildFile(args[0]);

    if(! in.existsAsFile())
    {
        std::cout << "Input file: " << in.getFullPathName().toUTF8().getAddress();
        std::cout << " does not exist" << std::endl;
        return 1;
    }
    
    XmlElement* mdl = XmlDocument::parse(in);

    String dspString = generateDspString(*mdl);

    DBG(dspString);
    delete mdl;

    if(writeStringToFile(dspString, args[1]))
//    if(true)
        return 0;
    else
        return 1;
}
