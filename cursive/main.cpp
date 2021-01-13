
#include <cursive/cursive.h>
#include <cursive/console_renderer.h>

#include <iostream>
#include <fstream>

using namespace cursive;


int main()
{
    std::ifstream test("C:\\workspace\\cursive\\brainstorm\\markdown_test.txt");
    
    std::string str((std::istreambuf_iterator<char>(test)), std::istreambuf_iterator<char>());

    auto doc = cursive::parse(str);
    std::ofstream ofs("C:\\workspace\\cursive\\brainstorm\\parse_out.txt");
    console_renderer::render(doc, ofs);

}