#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

namespace File_utils
{
    struct Is_guard
    {
        ifstream& s;
        ifstream::iostate old_e {s.exceptions()};
        Is_guard(ifstream& ss, ifstream::iostate e) : s{ss} {s.exceptions(s.exceptions()|e);}
        ~Is_guard() {s.exceptions(old_e);}
    };

    size_t size_of_file(ifstream&);
    ifstream open_input(const string&, const ios_base::openmode m = ios_base::in);
    string file_to_string(ifstream&);
	void traverse_dir(const string&, const string&);
	bool is_directory(const string&);
	string open_and_read_content(const string&);
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
	string& replace_tab_and_eol(string& s);
}

namespace Cmd_args
{
	bool is_path_valid(const string&);
	bool is_pattern_valid(const string&);
}

#endif
