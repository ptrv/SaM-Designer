/*
  ==============================================================================

    RegularExpression.h
    Created: 20 Sep 2012 7:14:35pm
    Author:  peter

  ==============================================================================
*/

#ifndef __REGULAREXPRESSION_H_197041BB__
#define __REGULAREXPRESSION_H_197041BB__

//==============================================================================
/**
    This class is designed around RE2 library for regular expression
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
        AlertWindow::showMessageBox(AlertWindow::WarningIcon,T(""),T("not found anything"));

    @endcode

*/
class RegularExpression
{
public:

	RegularExpression();
	RegularExpression( const String& pattern );
	~RegularExpression();

    bool fullMatch(const String& subject);
    bool fullMatch(const String& pattern, const String& subject);
    bool fullMatchValues(const String& subject, StringArray& result, int numResults);
    bool fullMatchValues(const String& pattern, const String& subject,
                         StringArray& result, int numResults);

private:

	String pattern;
	String subject;

};

#endif  // __REGULAREXPRESSION_H_197041BB__
