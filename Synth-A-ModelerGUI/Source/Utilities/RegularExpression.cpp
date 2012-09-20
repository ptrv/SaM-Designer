/*
  ==============================================================================

    RegularExpression.cpp
    Created: 20 Sep 2012 7:14:35pm
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
#include "../JuceLibraryCode/JuceHeader.h"

#include "RegularExpression.h"

#include "re2/re2.h"

//==============================================================================
RegularExpression::RegularExpression() :
	pattern (String::empty),
	subject (String::empty)
{
}

RegularExpression::RegularExpression( const String& pattern ) :
	pattern(pattern),
	subject(String::empty)
{
}

RegularExpression::~RegularExpression()
{
}

bool RegularExpression::fullMatch( const String& pattern_, const String& subject_ )
{
	pattern = pattern_;
	return fullMatch(subject_);
}

bool RegularExpression::fullMatch( const String& subject_ )
{
	subject = subject_;
    RE2 regexPattern(pattern.toUTF8().getAddress());

    bool isMatch = RE2::FullMatch(subject.toUTF8().getAddress(),
                                  regexPattern);

	return isMatch;
}

bool RegularExpression::fullMatchValues(const String& pattern_, const String& subject_,
                                        StringArray& result, int numResults)
{
	pattern = pattern_;
	return fullMatchValues(subject_, result, numResults);
}

bool RegularExpression::fullMatchValues(const String& subject_, StringArray& result,
                                        int numResults)
{
	subject = subject_;

    result.clear();
    
    RE2 regexPattern(pattern.toUTF8().getAddress());

    std::vector<std::string> vals(numResults, "");
    RE2::Arg* args[numResults];
    OwnedArray<RE2::Arg> ownedArgs;
    for (int i = 0; i < numResults; ++i)
    {
        RE2::Arg* newArg = new RE2::Arg(&vals[i]);
        ownedArgs.add(newArg);
        args[i] = ownedArgs[i];
    }
    bool isMatch = RE2::FullMatchN(subject.toUTF8().getAddress(),
                                   regexPattern, args, numResults);

    for (int i = 0; i < numResults; ++i)
    {
        result.add(vals[i].c_str());
    }

    ownedArgs.clear();
    return isMatch;
}