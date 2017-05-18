#include <iostream> // std::cerr, std::cout
#include <string>
#include <sstream>
#include <exception>
#include <fstream>  // std::ifstream
#include <chrono>

#include "file_handler.h"
#include "file_utils.h"

using namespace std;

void find_in_string(const string& fname, string& text, const string& pattern)
{
    for (Find_handler fh {text}; fh.find_in_text(pattern);)
    {
        string s {fh.make_report_for(fname)};

        cout << s;
    }
}

void is_valid_pattern(const string&);

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

