#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <fstream>
#include <string>
#include <ios>

// user interface
namespace File_utils
{
    using std::ifstream;
    using std::string;
    using std::ios_base;
    using std::istream;

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

#endif

