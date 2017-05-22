#include <fstream>
#include <string>
#include <iostream>
#include <exception>
#include <system_error>
#include <Windows.h>
#include <vector>
#include <thread>

#include "main.h"

size_t File::size_of_file(ifstream& ifs)
{	
	// max macro clashes with numeric_limits<streamsize>::max()
	#undef max

	// position of the current character
	streampos old_p{ ifs.tellg() };

	// skip characters until EOF or max streamsize is reached
	ifs.ignore(numeric_limits<streamsize>::max());

	// characters count in file
	streamsize n{ ifs.gcount() };

	// move back to the old position
	ifs.seekg(old_p);

    return static_cast<size_t>(n);
}

ifstream File::open_input(const string& name, const ifstream::openmode mode )
{
    ifstream ifs;

	// set ifstream to throw in case of error
    Is_guard guard {ifs, ifstream::failbit | ifstream::badbit};
    // throw if open failed
    ifs.open(name, mode);

    return ifs;
}

string File::file_to_string(ifstream& ifs)
{
	size_t n{ size_of_file(ifs) };

	string s;
	s.resize(n);

	// copy file content into string
	ifs.read(&s[0], s.size());

    return s;
}

void File::traverse_dir(const string& dirpath, const string& pattern)
{
	WIN32_FIND_DATA file;
	vector<thread> threads;	// spawned threads

	// *.* - any name, any extension
	HANDLE find_h{ FindFirstFile((dirpath + "\\*.*").c_str(), &file) };
	if (find_h == INVALID_HANDLE_VALUE)
	{
		cerr << "Cannot find path '" << dirpath << '\n';
		return;
	}

	// get all files in directory
	// in case of other directory - recursively traverse
	do
	{
		const string name{ file.cFileName };

		// skip current & parent directory
		if (name == "." || name == "..")
			continue;

		// extend path to the file
		string path{ dirpath + "\\" + name };

		// directory
		if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			traverse_dir(path, pattern);
		}
		else
		{
			// regular file
			string text{ File::open_and_read_content(path) };
			if (!text.empty())
			{
				// move name, text because thread can overlive the variable
				threads.emplace_back(Search::find_str_and_report, move(name), move(text), ref(pattern));
			}
		}

	} while (FindNextFile(find_h, &file));

	// wait for all threads til finish
	for (auto &t : threads)
		if (t.joinable())
			t.join();

	FindClose(find_h);
}

bool File::is_directory(const string& s)
{
	DWORD a{ GetFileAttributes(s.c_str()) };

	if (a == INVALID_FILE_ATTRIBUTES)
	{
		// narrowing - need int
		error_code ec{static_cast<int>(GetLastError()), generic_category() };
		throw system_error{ec, "Cannot recognize file"};
	}

	// is directory ?
	if (a & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}

string File::open_and_read_content(const string& s)
try
{
	// open file
	ifstream ifs{ File::open_input(s) };

	// get file content
	string text{ File::file_to_string(ifs) };
		
	return text;
}
catch (const ifstream::failure e)
{
	cerr << "Caught ifstream::failure when reading file '" 
		 << s << "' meaning " << e.what() << '\n';
	return string{};
}
catch (const length_error e)
{
	cerr << "Caught length_error meaning " << e.what() << '\n';
	return string{};
}