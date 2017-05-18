#include <iostream> // std::cerr, std::cout
#include <string>
#include <sstream>
#include <exception>
#include <fstream>  // std::ifstream
#include <chrono>

#include "file_handler.h"

using namespace std;

struct Is_guard
{
    istream& s;
    ios_base::iostate old_e {s.exceptions()};
    Is_guard(istream& ss, ios_base::iostate e) : s{ss} {s.exceptions(s.exceptions()|e);}
    ~Is_guard() {s.exceptions(old_e);}
};

ifstream open_input(const string& name, const ios_base::openmode mode = ios_base::in)
{
    ifstream ifs; 

    Is_guard guard {ifs, ios_base::failbit | ios_base::badbit};
    // throw if open failed
    ifs.open(name, mode);

    return ifs;
}

long size_of_file(ifstream& ifs)
{
    // move to the last byte
    ifs.seekg(0, ifs.end);
    ios_base::streampos size {ifs.tellg()};
    // move to the begin
    ifs.seekg(0, ifs.beg);

    return size;
}

string file_to_string(ifstream& ifs)
{
    long size {size_of_file(ifs)};

    string s;
    // may throw bad_alloc, length_error
    s.resize(size);

    ifs.read(&s[0], size);

    return s;
}

void find_in_string(const string& fname, string& text, const string& pattern)
{
    for (Find_handler fh {text}; fh.find_in_text(pattern);)
    {
        string s {fh.make_report_for(fname)};

        cout << s;
    }
}

void is_valid_pattern(const string& s)
{
    if (s.length() > 128)
    {
        throw runtime_error {"Pattern is too long! (max 128 chars)"};
    }

    if (s.empty())
    {
        throw runtime_error {"Pattern is an empty string!"};
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " file pattern\n";
        return 1;
    }

    const string& filename {argv[1]};
    const string& pattern {argv[2]};

    // open file
    ifstream ifs;
    try
    {
        ifs = open_input(filename);
    }
    //catch (const ios_base::failure e)
    catch (const exception e)
    {
        cerr << "Cannot open " << filename << ": " << e.what() << '\n';
        return 1;
    }

    // validate pattern
    try
    {
        is_valid_pattern(pattern);
    }
    catch (const runtime_error e)
    {
        cerr << e.what() << '\n';
        return 1;
    }

    // file content into string cause of find 
    string text;
    try
    {
        text = file_to_string(ifs);
    }
    // mem allocation can fail
    catch (const bad_alloc e)
    {
        cerr << e.what() << '\n';
        return 1;
    }
    // can exceed max allowed string size 
    catch (const length_error e)
    {
        cerr << e.what() << '\n';
        return 1;
    }

    using namespace chrono;

    // time measurement
    auto t1 = system_clock::now();

    find_in_string(filename, text, pattern);
    
    auto t2 = system_clock::now();

    auto t3 = duration_cast<milliseconds>(t2-t1);
    cerr << "Lasted: " << t3.count() << " milliseconds\n";

    return 0;
}

