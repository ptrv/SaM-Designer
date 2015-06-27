/*
  ==============================================================================

    RegularExpression.h
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

#ifndef __REGULAREXPRESSION_H_197041BB__
#define __REGULAREXPRESSION_H_197041BB__

#include "JuceHeader.h"

namespace synthamodeler
{
//==============================================================================
/*
    This class is designed around PCRE library for regular expression
    parsing and evaluating

    e.g. @code
    RegularExpression re("\\d{3}");
    int found = re.match("123.345");

    if ( found >= 0 )
    {
        int o, e;
        for( int i=0; i<found; i++ )
        {
            String s = "";
            String result = re.getStringOffsetSize(i,o,e);

            s << "'" << result << "' - (" << o << "," << e << ")" ;
            AlertWindow::showMessageBox(AlertWindow::WarningIcon,T(""),s);
        }
    }
    else
        AlertWindow::showMessageBox(AlertWindow::WarningIcon,T(""),T("not found anything"))
 */
class RegularExpression
{
public:

    enum {
        OVECOUNT = 30,
        compileFailed = -100,
        bufferSizeError = -101,
        matchNotFound = -102,
        matchFindError = -103
    };

    RegularExpression();
    RegularExpression( const String& pattern_);
    ~RegularExpression();

    bool fullMatch(const String& subject);
    bool fullMatch(const String& pattern, const String& subject);
    bool fullMatchValues(const String& subject, StringArray& result, int numResults);
    bool fullMatchValues(const String& pattern, const String& subject,
                         StringArray& result, int numResults);
    // bool partialMatch(const String& subject);
    // bool partialMatch(const String& pattern, const String& subject);
    void findAndConsume(const String& subject, StringArray& result);
    void findAndConsume(const String& pattern, const String& subject,
                        StringArray& result);

    int match( const String& subject );
    int match( const String& pattern, const String& subject );
    String getStringOffsetSize( int num, int& start, int& end );

private:

    class ExpressionIndex
    {
    public:
        ExpressionIndex(int s, int e)
            : start(s), end(e) { }
        int start;
        int end;
    };

    int numSubStrings;

    OwnedArray<ExpressionIndex> index;

    String pattern;
    String subject;

    RegularExpression(const RegularExpression& other);
};
}

#endif  // __REGULAREXPRESSION_H_197041BB__
