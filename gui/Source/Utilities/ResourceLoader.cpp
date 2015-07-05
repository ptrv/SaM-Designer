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

#include "ResourceLoader.h"

#include "Models/ObjectIDs.h"


using namespace synthamodeler;

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
	objectIcons.set(Ids::mass, "mass.svg");
	objectIcons.set(Ids::port, "port.svg");
	objectIcons.set(Ids::resonators, "resonator.svg");
	objectIcons.set(Ids::ground, "ground.svg");
	objectIcons.set(Ids::link, "link.svg");
	objectIcons.set(Ids::touch, "touch.svg");
	objectIcons.set(Ids::pluck, "pluck.svg");
	objectIcons.set(Ids::termination, "termination.svg");
	objectIcons.set(Ids::waveguide, "waveguide.svg");
	objectIcons.set(Ids::junction, "junction.svg");
    objectIcons.set(Ids::audioout, "audioout.svg");
    objectIcons.set(Ids::display, "display.svg");
}
Drawable* ResourceLoader::getDrawableForObjectId(const Identifier& objId)
{
	return createSVGDrawable(objectIcons[objId]);
}
Drawable* ResourceLoader::createSVGDrawable(const String& filename, const bool createCopy)
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

    if (createCopy)
    {
        return iconsFromZipFile [iconNames.indexOf (filename)]->createCopy();
    }
    else
    {
        return iconsFromZipFile [iconNames.indexOf (filename)];//->createCopy();
    }
}

ToolbarButton* ResourceLoader::createButtonFromZipFileSVG (const int itemId,
                                                           const String& text,
                                                           const String& filename)
{
    Drawable* image = createSVGDrawable(filename, true);
    return new ToolbarButton(itemId, text, image, nullptr);
}

Path ResourceLoader::getPathForLink(float /*x*/, float /*y*/, float w, float h)
{
    Path linkPath;
    float step = w / 20.0f;
    linkPath.startNewSubPath(0,0);
    linkPath.addEllipse(0,0, w, h);
    linkPath.addEllipse(step,step, w-step*2, h-step*2);
    linkPath.addEllipse(step*2, step*2, w-step*4, h-step*4);
    linkPath.addEllipse(step*3, step*3, w-step*6, h-step*6);
    linkPath.addEllipse(step*4, step*4, w-step*8, h-step*8);
    linkPath.applyTransform(AffineTransform::translation(-w/2, 0));
    return linkPath;
}
Path ResourceLoader::getPathForPluck(float /*x*/, float /*y*/, float w, float h)
{
    Path touchPath;
    float lineDist = w * 0.7f;
    float nw = w/2.0f;
    float nh = h/2.0f;
    touchPath.startNewSubPath(0, 0);
    touchPath.addRectangle(0, 0, w, h);
    touchPath.startNewSubPath(lineDist, 0);
    touchPath.lineTo(lineDist, h);
    touchPath.applyTransform(AffineTransform::translation(-nw, -nh));
    return touchPath;
}
Path ResourceLoader::getPathForTouch(float /*x*/, float /*y*/, float w, float h)
{
    Path pluckPath;
    float step = w / 20.0f;
    float offset = w / 6;
    pluckPath.addRectangle(0, 0, w, h);
//    PathStrokeType stroke2(0.4f);
//    stroke2.createStrokedPath(pluckPath, pluckPath);
    pluckPath.addEllipse(offset+step, offset+step, w-(offset*2+step*2), w-(offset*2+step*2));
    pluckPath.addEllipse(offset+step*2, offset+step*2, w-(offset*2+step*4), w-(offset*2+step*4));
    pluckPath.addEllipse(offset+step*3, offset+step*3, w-(offset*2+step*6), w-(offset*2+step*6));
    pluckPath.addEllipse(offset+step*4, offset+step*4, w-(offset*2+step*8), w-(offset*2+step*8));
//    pluckPath.addLineSegment(Line<float>(0, 25, 5, 25), 10.0f);
    pluckPath.applyTransform(AffineTransform::translation(-w/2, -h/2));

    Path line;
    line.addLineSegment(Line<float>(Point<float>(w/2+(offset*2-step), h/2),
                                    Point<float>(w, h/2)), 0.8f);
    line.applyTransform(AffineTransform::translation((-w/2), -h/2));
    pluckPath.addPath(line);

    PathStrokeType stroke(0.8f);
    stroke.createStrokedPath(pluckPath, pluckPath);

    return pluckPath;
}
Path ResourceLoader::getPathForLinkId(const Identifier& linkId, float x, float y, float w, float h)
{
    if(linkId == Ids::link || linkId == Ids::detent ||
       linkId == Ids::softeninglink || linkId == Ids::stiffeninglink)
    {
        return getPathForLink(x, y, w, h);
    }
    else if(linkId == Ids::touch || linkId == Ids::pulsetouch)
    {
        return getPathForTouch(x, y, w, h);
    }
    else if(linkId == Ids::pluck)
    {
        return getPathForPluck(x, y, w, h);
    }
    else
    {
        return Path();
    }
}
