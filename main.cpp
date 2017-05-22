#include <iostream>
#include <string>
#include <system_error>
#include <exception>
#include <shlwapi.h> // PathFileExists

#include "main.h"

int main(int argc, char* argv[])
try
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " file_or_dir_path pattern\n";
        return 1;
    }

	const string& path{ argv[1] };
	Arguments::validate_path(path);

	const string& pattern{ argv[2] };
	Arguments::validate_pattern(pattern);

	if (File::is_directory(path))
	{
		File::traverse_dir(path, pattern);

		// wait for all threads till finish
		for (auto &t : File::threads)
			if (t.joinable())
				t.join();
	}
	else
	{
		string text{ File::open_and_read_content(path) };

		if (!text.empty())
		{
			// get file name
			string name{ path };
			name.erase(0, path.find_last_of('\\') + 1);

			Search::find_str_and_report(name, text, pattern);
		}
	}

    return 0;
}
catch (const bad_alloc e)
{
	cerr << "Caugth bad allocation: " << e.what() << '\n';
	return 1;
}
catch (const system_error e)
{
	cerr << "Caught system_error with code: " << e.code()
		 << " meaning " << e.what() << '\n';
	return 1;
}
catch (const runtime_error e)
{
	cerr << "Caught runtime_error meaning " << e.what() << '\n';
	return 1;
}

void Arguments::validate_path(const string& s)
{
	if (!PathFileExists(s.c_str()))
	{
		error_code ec{ static_cast<int>(GetLastError()), generic_category() };
		throw system_error{ ec, "Invalid path" };
	}
}

void Arguments::validate_pattern(const string& s)
{
	// length must be <= 128
	if (s.empty() || s.length() > 128)
	{
		throw runtime_error{ "Invalid pattern length! (empty or length > 128)\n" };
	}
}