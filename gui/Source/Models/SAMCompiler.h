/*
  ==============================================================================

    SAMCompiler.h
    Created: 19 Oct 2012 3:48:21pm
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

#ifndef __SAMCOMPILER_H_7CDF5BCC__
#define __SAMCOMPILER_H_7CDF5BCC__


namespace synthamodeler
{

class SAMCompiler
{
private:
    SAMCompiler();
    ~SAMCompiler();

    static const char* wgL;
    static const char* wgR;
    static const char* wgLp;
    static const char* wgRp;
    static const char* jTO;
    static const char* jOutputs;

    struct MassLinkRef
    {
        String massId;
        StringArray linkRefs;
        bool isPort;
    };

    struct WgWithSuffixes
    {
        String wgId;
        StringArray wgSuffixes;
        bool termRight;
        StringArray wgParams;
    };

    static int containsMassLinkRef(const OwnedArray<MassLinkRef>& mlf, const String& mId);
    static int containsWgRef(const OwnedArray<WgWithSuffixes>& wws, const String& wId);
    static ValueTree getJunctionLink(ValueTree linksTree, ValueTree jnctTree);
    static ValueTree getWgForJunct(ValueTree waveguides, ValueTree j);
public:
    static const String compile(ValueTree mdlRoot_);
};

}

#endif  // __SAMCOMPILER_H_7CDF5BCC__
