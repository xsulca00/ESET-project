#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <fstream>

using namespace std;

namespace File
{
	// RAII
	// set ifstream to throw in case of error
	// set old exception flags in case of any throw
    struct Is_guard
    {
        ifstream& s;
        ifstream::iostate old_e {s.exceptions()};
        Is_guard(ifstream& ss, ifstream::iostate e) : s{ss} {s.exceptions(s.exceptions()|e);}
        ~Is_guard() {s.exceptions(old_e);}
    };

	// implementer interface
    size_t size_of_file(ifstream&);
    ifstream open_input(const string&, const ifstream::openmode m = ifstream::in);
    string file_to_string(ifstream&);

	// user interface
	void traverse_dir(const string&, const string&);
	bool is_directory(const string&);
	string open_and_read_content(const string&);
}

namespace Search
{
	// handle substring search in file
	// with prefix & suffix correct generating
    class Find_handler
    {
    public:
        Find_handler(string&);

        bool find_in_text(const string&);
        string make_report_for(const string&) const;
    private:
		void replace_tab_and_eol(string&);

        string& text;

        // where is pattern in string
        size_t offset;

        string prefix;
        string suffix;
    };

	// get all pattern positions in file and report
    void find_str_and_report(const string fname, string text, const string& pattern);
}

namespace Arguments
{
	void validate_path(const string&);
	void validate_pattern(const string&);
}

#endif
