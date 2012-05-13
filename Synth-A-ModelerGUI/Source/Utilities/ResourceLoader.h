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


class ResourceLoader
{
public:
	ResourceLoader();
	~ResourceLoader();

	juce_DeclareSingleton (ResourceLoader, false)

	Drawable* getDrawableForId(const Identifier& objId);
	Drawable* createSVGDrawable(const String& filename);

private:
	void initObjectIconMap();
	HashMap<String,String> objectIcons;
	StringArray iconNames;
	OwnedArray<Drawable> iconsFromZipFile;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResourceLoader);
};


#endif  // __RESOURCELOADER_H_E0EF3E32__
