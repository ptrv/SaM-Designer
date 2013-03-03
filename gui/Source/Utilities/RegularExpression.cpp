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

using namespace synthamodeler;
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
    std::string pttr(pattern.getCharPointer().getAddress());
//    RE2 regexPattern(pttr);

    std::string subj(subject.getCharPointer().getAddress());
    return RE2::FullMatch(subj, pttr);
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

    std::string pttr = pattern.getCharPointer().getAddress();
//    RE2 regexPattern(pttr);

    std::vector<std::string> vals(numResults, "");
	std::vector<RE2::Arg const*> args;
    for (int i = 0; i < numResults; ++i)
    {
        RE2::Arg* newArg = new RE2::Arg(&vals[i]);
        args.push_back(newArg);
    }

    std::string subj = subject.getCharPointer().getAddress();
//    DBG(pattern.toUTF8().getAddress());
//    DBG(subj.c_str());
	bool isMatch = RE2::FullMatchN(subj, pttr, &args[0], numResults);

    for (int i = 0; i < numResults; ++i)
    {
        result.add(vals[i].c_str());
		delete args[i];
		args[i] = nullptr;
    }

    return isMatch;
}

bool RegularExpression::partialMatch( const String& pattern_, const String& subject_ )
{
	pattern = pattern_;
	return fullMatch(subject_);
}

bool RegularExpression::partialMatch( const String& subject_ )
{
	subject = subject_;
    RE2 regexPattern(pattern.toUTF8().getAddress());

    return RE2::PartialMatch(subject.toUTF8().getAddress(),
                             regexPattern);
}
void RegularExpression::findAndConsume(const String& pattern_, const String& subject_,
                                       StringArray& result)
{
    pattern = pattern_;
    findAndConsume(subject_, result);
}

void RegularExpression::findAndConsume(const String& subject_, StringArray& result)
{
    subject = subject_;
    RE2 regex(pattern.toUTF8().getAddress());
    
    re2::StringPiece input(subject.toUTF8().getAddress());

    std::string word;

    while (RE2::Consume(&input, regex, &word))
    {
        if(word.empty())
            break;
        result.add(word.c_str());
    }
}
