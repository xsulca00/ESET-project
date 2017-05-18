#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <fstream>
#include <string>
#include <ios>

using std::ifstream;
using std::string;
using std::ios_base;

long size_of_file(ifstream&);
ifstream open_input(const string&, const ios_base::openmode m = ios_base::in);
string file_to_string(ifstream&);

#endif

