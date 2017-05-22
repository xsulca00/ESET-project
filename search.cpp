#include <string>
#include <iostream>
#include <sstream>

#include "main.h"

Search::Find_handler::Find_handler(string& ttext)
        : text{ttext}, offset{0}, prefix{}, suffix{}
{

}

void Search::Find_handler::replace_tab_and_eol(string& s)
{
	// find '\t' or '\n' and replace
	size_t found = s.find_first_of("\t\n");
	while (found != string::npos)
	{
		if (s[found] == '\t')
			s.replace(found, 1, "\\t");
		else if (s[found] == '\n')
			s.replace(found, 1, "\\n");

		found = s.find_first_of("\t\n", found + 1);
	}
}

bool Search::Find_handler::find_in_text(const string& s)
{
    offset = text.find(s, offset);

    if (offset != string::npos)
    {
        // suffix position
        size_t so {offset+s.length()};

		// prefix is shorther than 3
        if (offset < 3)
            prefix = text.substr(0, offset);
        else
            prefix = text.substr(offset-3, 3);

        suffix = text.substr(so, 3);

		// replace newlines and tabs with '\n' and '\t'
		replace_tab_and_eol(prefix);
		replace_tab_and_eol(suffix);

        // move on to next char
        offset++;

        return true;
    }

    return false;
}

string Search::Find_handler::make_report_for(const string& fname) const
{
    ostringstream ss;

	// move back to position of pattern (offset-1)
    ss << fname
       << "(" << offset-1 << "): "
       << prefix
       << "..."
       << suffix << '\n';

    return ss.str();
}

void Search::find_str_and_report(const string& fname, string& text, const string& pattern)
{
    for (Find_handler fh {text}; fh.find_in_text(pattern);)
    {
        string s {fh.make_report_for(fname)};

		cout << s;
    }
}


 