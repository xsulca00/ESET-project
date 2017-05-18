#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

using std::string;

class Find_handler 
{
public:
    Find_handler(string&);

    bool find_in_text(const string&);
    string make_report_for(const string&) const;
private:
    string& text;
    // where is pattern in string
    size_t offset;

    string prefix;
    string suffix;
};

#endif
