
#include "../inc/document.h"
#include "../inc/diagnostics.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>


class arglist
{
	typedef std::pair<char, std::string> arg;
	std::vector<arg> arglist_;
public:
	arglist(int argc, const char **argv)
	{
		// iterate through the arguments looking for - check if next is the token we want
		// get next argument as option
		// yes you can have multiple inserted of the same tag, you'll only ever get the last
		// one

		for (int i = 0; i < argc; ++i)
		{
			if (argv[i][0] == '-')
			{
				if (i < argc && strlen(argv[i]) > 1)
				{
					char tag = argv[i][1];
					arglist_.push_back(std::make_pair(tag, std::string(argv[++i])));
				}
			}
		}
	}
	std::string get(char tag)
	{
		auto i = std::find_if(std::begin(arglist_), std::end(arglist_), [&](std::pair<char, std::string> &arg)
		{
			return arg.first == tag;
		});
		if (i == std::end(arglist_))
			throw std::runtime_error("argument not found");
		return i->second;
	}
};


/////
/*
  Usage - ./mdp [markdown file] [output html]
*/
/////

int main(int argc, const char **argv)
{
	try
	{
		arglist al(argc, argv);
		const std::string infile_s = al.get('i');
		const std::string outfile_s = al.get('o');

		std::ifstream infs(infile_s);
		if (!infs.good())
			throw std::runtime_error("failed to open markdown file");

		std::ofstream outfs(outfile_s);
		if (!outfs.good())
			throw std::runtime_error("failed to open destination file");

		tdp::document doc;
		tdp::parse(infs, doc);
		tdp::print(doc.base(), outfs);

	}
	catch (std::exception &e)
	{
		std::stringstream ss;
		ss << "error occured - " << e.what();
		diagnostics::log_err(ss.str());
		return -1; 
	}
	catch (...)
	{
		diagnostics::log_err("unknown error occured");
		return -1;
	}
	return 0;
    
}
