#include <iostream> // std::cerr, std::cout
#include <string> // std::string
#include <chrono> // chrono::system_clock::now(), chrono::duration_cast

#include "main.h"

bool Cmd_args::is_path_valid(const string& s)
{
	if (s.empty())
	{
		return false;
	}

	return true;
}

bool Cmd_args::is_pattern_valid(const string& s)
{
	// length must be <= 128
	if (s.length() > 128)
	{
		return false;
	}

	if (s.empty())
	{
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
try
{
    if (argc != 3)
    {
        cerr << argc << "Usage: " << argv[0] << " file_or_dir_path pattern\n";
        return 1;
    }

	const string& path{ argv[1] };
	if (!Cmd_args::is_path_valid(path))
	{
		cerr << "Invalid file or directory path!\n";
		return 1;
	}

	const string& pattern{ argv[2] };
	if (!Cmd_args::is_pattern_valid(pattern))
	{
		cerr << "Invalid pattern!\n";
		return 1;
	}

	// time measurement
	auto t1 = chrono::system_clock::now();

	if (File_utils::is_directory(path))
	{
		File_utils::traverse_dir(path, pattern);
	}
	else
	{
		string text{ File_utils::open_and_read_content(path) };

		string name{ path };
		// get only file name
		name.erase(0, path.find_last_of('\\')+1);

		if (!text.empty())
			String_seeking::find_and_print_for(name, text, pattern);
	}

	auto t2 = chrono::system_clock::now();

	auto t3 = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
	cerr << "Lasted: " << t3.count() << " milliseconds\n";

    return 0;
}
catch (const bad_alloc e)
{
	// mem allocation can fail
	cerr << "Memory allocation failed!\n";
	return 1;
}
catch (const system_error e)
{
	cerr << "Caught system_error with code: " << e.code()
		<< " meaning " << e.what() << '\n';
	return 1;
}



