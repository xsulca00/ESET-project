#include <iostream> // std::cerr, std::cout
#include <string>
#include <sstream>
#include <exception>
#include <fstream>  // std::ifstream
#include <chrono>

#include "main.h"

void Arg_checking::validate_pattern(const string& s)
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
    using namespace std;

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
        ifs = File_utils::open_input(filename);
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
        Arg_checking::validate_pattern(pattern);
    }
    catch (const runtime_error e)
    {
        cerr << e.what() << '\n';
        return 1;
    }

    // file content
    string text;
    try
    {
        text = File_utils::file_to_string(ifs);
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

    String_seeking::find_and_print_for(filename, text, pattern);
    
    auto t2 = system_clock::now();

    auto t3 = duration_cast<milliseconds>(t2-t1);
    cerr << "Lasted: " << t3.count() << " milliseconds\n";

    return 0;
}


