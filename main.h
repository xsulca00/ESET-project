#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

using namespace std;

namespace File_utils
{
    struct Is_guard
    {
        istream& s;
        ios_base::iostate old_e {s.exceptions()};
        Is_guard(istream& ss, ios_base::iostate e) : s{ss} {s.exceptions(s.exceptions()|e);}
        ~Is_guard() {s.exceptions(old_e);}
    };

    long size_of_file(ifstream&);
    ifstream open_input(const string&, const ios_base::openmode m = ios_base::in);
    string file_to_string(ifstream&);
}

namespace String_seeking
{
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

    void find_and_print_for(const string& fname, string& text, const string& pattern);
}

namespace Arg_checking
{
    void validate_pattern(const string&);
}

#endif
