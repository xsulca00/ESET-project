#include <fstream>
#include <string>
#include <ios>

#include "file_utils.h"

using std::ifstream;
using std::string;
using std::ios_base;
using std::istream;

long File_utils::size_of_file(ifstream& ifs)
{
    // move to the last byte
    ifs.seekg(0, ifs.end);
    ios_base::streampos size {ifs.tellg()};
    // move to the begin
    ifs.seekg(0, ifs.beg);

    return size;
}

ifstream File_utils::open_input(const string& name, const ios_base::openmode mode )
{
    ifstream ifs;

    Is_guard guard {ifs, ios_base::failbit | ios_base::badbit};
    // throw if open failed
    ifs.open(name, mode);

    return ifs;
}

string File_utils::file_to_string(ifstream& ifs)
{
    long size {size_of_file(ifs)};

    string s;
    // may throw bad_alloc, length_error
    s.resize(size);

    ifs.read(&s[0], size);

    return s;
}


