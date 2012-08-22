/*
  ==============================================================================

    ResourceLoader.h
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

#ifndef __RESOURCELOADER_H_E0EF3E32__
#define __RESOURCELOADER_H_E0EF3E32__

/**
 * Class for loading resources like icons.
 */
class ResourceLoader
{
public:
	ResourceLoader();
	~ResourceLoader();

	juce_DeclareSingleton (ResourceLoader, false)

	/**
	 * Gets a Drawable for the object Id (i.e. Ids::mass)
	 * @param objId		Id of object
	 * @return			a Drawable
	 */
	Drawable* getDrawableForId(const Identifier& objId);
	/**
	 * Creates a SVG drawable from a zip file.
	 *
	 * @param filename	string filename of SVG image
	 * @return			a Drawable
	 */
	Drawable* createSVGDrawable(const String& filename);
    
    Path getPathForLinkId(const Identifier& linkId, float x, float y, float w, float h);

private:
	void initObjectIconMap();
    Path getPathForLink(float x, float y, float w, float h);
    Path getPathForTouch();
    Path getPathForPluck();
    
	HashMap<String,String> objectIcons;
	StringArray iconNames;
	OwnedArray<Drawable> iconsFromZipFile;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResourceLoader);
};


#endif  // __RESOURCELOADER_H_E0EF3E32__
