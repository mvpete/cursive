
#include <cursive/cursive.h>
#include <cursive/console_renderer.h>
#include "html_renderer.h"
#include <cursive/cursive.pdf.h>

#include <iostream>
#include <fstream>

using namespace cursive;

std::string to_string(std::ifstream &ifs)
{
    std::string s;
    while (ifs.good())
    {
        char c = { 0 };
        ifs.read(&c,1);
        s.push_back(c);
    }
    return s;
}

int main(int argc, const char **argv)
{
    if (argc < 3)
        return -1;

    const std::string in_file_name(argv[1]);
    const std::string out_file_name(argv[2]);
    
    std::ifstream ifs(in_file_name);
    auto s = to_string(ifs);
    auto doc = cursive::parse(s);

    std::ofstream ofs(out_file_name);
    cursive::render<char, html_renderer>(doc, ofs);

    return 0;
}