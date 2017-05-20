#include <string>
#include <iostream>
#include <sstream>

#include "main.h"

String_seeking::Find_handler::Find_handler(string& ttext)
        : text{ttext}, offset{0}, prefix{}, suffix{}
{

}

bool String_seeking::Find_handler::find_in_text(const string& s)
{
    offset = text.find(s, offset);

    if (offset != string::npos)
    {
        // suffix offset
        size_t so {offset+s.length()};

        if (offset < 3)
            prefix = text.substr(0, offset);
        else
            prefix = text.substr(offset-3, 3);

        suffix = text.substr(so, 3);

        // move on to next char
        offset++;

        return true;
    }

    return false;
}

string String_seeking::Find_handler::make_report_for(const string& fname) const
{
    using std::ostringstream;

    ostringstream ss;

    ss << fname
       << "(" << offset << "): "
       << prefix
       << "..."
       << suffix << '\n';

    return ss.str();
}

void String_seeking::find_and_print_for(const string& fname, string& text, const string& pattern)
{
    for (Find_handler fh {text}; fh.find_in_text(pattern);)
    {
        string s {fh.make_report_for(fname)};

        cout << s;
    }
}


