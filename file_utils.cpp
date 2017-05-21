#include <fstream>
#include <string>
#include <Windows.h>

#include "main.h"

ios_base::streampos File_utils::size_of_file(ifstream& ifs)
{
    // move to the last byte
	// problem with newline symbol
    ifs.seekg(0, ifs.end);
    ios_base::streampos size {ifs.tellg()};
    // move to the begin
    ifs.seekg(0, ifs.beg);

    return size;
}

ifstream File_utils::open_input(const string& name, const ios_base::openmode mode )
{
    ifstream ifs;

    Is_guard guard {ifs, ifstream::failbit | ifstream::badbit};
    // throw if open failed
    ifs.open(name, mode);

    return ifs;
}

string File_utils::file_to_string(ifstream& ifs)
{
    ios_base::streampos size {size_of_file(ifs)};

    string s;
    // may throw bad_alloc, length_error
    s.resize(static_cast<unsigned int>(size));

	// may fail (e.g. linux newline symbol)
	ifs.read(&s[0], size);

    return s;
}

void File_utils::traverse_dir(const string& dirpath, const string& pattern)
{
	WIN32_FIND_DATA file;

	// just add wildcard, need dirpath later
	HANDLE find_h{ FindFirstFile((dirpath + "\\*.*").c_str(), &file) };
	if (find_h == INVALID_HANDLE_VALUE)
	{
		cout << "Cannot find path '" << dirpath << '\n';
		return;
	}

	do
	{
		const string& name{ file.cFileName };

		// skip current & parent directory
		if (name == "." || name == "..")
			continue;

		string path{ dirpath + "\\" + name };

		// directory
		if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			traverse_dir(path, pattern);
		}
		else
		{
			// regular file
			string text{ File_utils::open_and_read_content(path) };	
			if (!text.empty())
				String_seeking::find_and_print_for(name, text, pattern);
		}

	} while (FindNextFile(find_h, &file));

	FindClose(find_h);
}

bool File_utils::is_directory(const string& s)
{
	DWORD a{ GetFileAttributes(s.c_str()) };

	if (a == INVALID_FILE_ATTRIBUTES)
	{
		// narrowing - need int
		error_code ec{static_cast<int>(GetLastError()), generic_category() };
		throw system_error{ec, "Cannot recognize file"};
	}

	if (a & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}

string File_utils::open_and_read_content(const string& s)
try
{
	// open file
	ifstream ifs{ File_utils::open_input(s) };

	// get file content
	string text{ File_utils::file_to_string(ifs) };
		
	return text;
}
catch (const ifstream::failure e)
{
	cerr << "Reading file '" << s << "' failed!: " << e.what() << '\n';
	return {};
}
catch (const length_error e)
{
	cerr << "File to string: " << e.what() << '\n';
	return {};
}



