#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

namespace File_search 
{
    using std::string;

    void find_and_print_for(const string& fname, string& text, const string& pattern);

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
}

#endif
