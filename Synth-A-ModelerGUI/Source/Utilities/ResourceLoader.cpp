/*
  ==============================================================================

    ResourceLoader.cpp
    Created: 13 May 2012 1:20:17am
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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Models/ObjectIDs.h"

#include "ResourceLoader.h"

ResourceLoader::ResourceLoader()
{
	initObjectIconMap();
}

ResourceLoader::~ResourceLoader()
{
	iconsFromZipFile.clear(true);
}

juce_ImplementSingleton (ResourceLoader)

void ResourceLoader::initObjectIconMap()
{
	objectIcons.set(Ids::mass.toString(),"mass.svg");
	objectIcons.set(Ids::port.toString(), "port.svg");
	objectIcons.set(Ids::resonator.toString(), "resonator.svg");
	objectIcons.set(Ids::ground.toString(), "ground.svg");
	objectIcons.set(Ids::link.toString(), "link.svg");
//	objectIcons.set(Ids::link.toString(), "link-no-connectors.svg");
	objectIcons.set(Ids::touch.toString(), "touch.svg");
	objectIcons.set(Ids::pluck.toString(), "pluck.svg");
	objectIcons.set(Ids::termination.toString(), "termination.svg");
	objectIcons.set(Ids::waveguide.toString(), "waveguide.svg");
	objectIcons.set(Ids::audioout.toString(), "audioout.svg");
//	objectIcons.set(Ids::junction.toString(), "waveguide.svg");
//	objectIcons.set(Ids::label.toString(), "waveguide.svg");
}
Drawable* ResourceLoader::getDrawableForId(const Identifier& objId)
{
	return createSVGDrawable(objectIcons[objId.toString()]);
}
Drawable* ResourceLoader::createSVGDrawable(const String& filename)
{
	initObjectIconMap();

    if (iconsFromZipFile.size() == 0)
    {
        // If we've not already done so, load all the images from the zip file..
        MemoryInputStream iconsFileStream (BinaryData::icons_zip, BinaryData::icons_zipSize, false);
        ZipFile icons (&iconsFileStream, false);

        for (int i = 0; i < icons.getNumEntries(); ++i)
        {
            ScopedPointer<InputStream> svgFileStream (icons.createStreamForEntry (i));

            if (svgFileStream != 0)
            {
                iconNames.add (icons.getEntry(i)->filename);
                iconsFromZipFile.add (Drawable::createFromImageDataStream (*svgFileStream));
            }
        }
    }

    return iconsFromZipFile [iconNames.indexOf (filename)];//->createCopy();
}
Path ResourceLoader::getPathForLink(float x, float y, float w, float h)
{
    Path linkPath;
    
    float step = w / 20.0f;
    linkPath.startNewSubPath(0,0);
    linkPath.addEllipse(0,0, w, h);
    linkPath.addEllipse(step,step, w-step*2, h-step*2);
    linkPath.addEllipse(step*2, step*2, w-step*4, h-step*4);
    linkPath.addEllipse(step*3, step*3, w-step*6, h-step*6);
//    linkPath.applyTransform(AffineTransform::translation(x, y));
    return linkPath;
}
Path ResourceLoader::getPathForTouch()
{
    return Path();
}
Path ResourceLoader::getPathForPluck()
{
    
}
Path ResourceLoader::getPathForLinkId(const Identifier& linkId, float x, float y, float w, float h)
{
    if(linkId == Ids::link)
    {
        return getPathForLink(x, y, w, h);
    }
    else if(linkId == Ids::touch)
    {
        
    }
    else if(linkId == Ids::pluck)
    {
        
    }
}