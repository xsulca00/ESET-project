#include <string>
#include <sstream>

#include "file_handler.h"

using std::string;

Find_handler::Find_handler(string& ttext)
        : text{ttext}, offset{0}, prefix{}, suffix{}
{

}

bool Find_handler::find_in_text(const string& s)
{
    offset = text.find(s, offset);

    if (offset != string::npos)
    {
        // suffix offset
        size_t so {offset+s.length()};

        prefix = text.substr((offset<3)?0:offset-3, (offset<3)?offset:3);
        suffix = text.substr(so, 3);

        // move on to next char
        offset++;

        return true;
    }

    return false;
}

string Find_handler::make_report_for(const string& fname) const
{
    using std::stringstream;

    stringstream ss;

    ss << fname
       << "(" << offset << "): "
       << prefix
       << "..."
       << suffix << '\n';

    return ss.str();
}

