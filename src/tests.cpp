
#include <iostream>
#include <sstream>
#include <fstream>

#include "inc/document.h"
#include "inc/diagnostics.h"

const int all_test_val=0;

const int document_structure_test_val = 1;
const int markdown_parse_test_val     = 2;

void markdown_parse_test()
{
    diagnostics::log_info("markdown_parse_test - start");
    
    diagnostics::log_info("opening 'test_markdown.txt'");
    std::ifstream markdown("test_markdown.txt");
    std::ofstream htmldown("test_htmlout.html");
    if(!markdown.good())
    {
	diagnostics::log_info("failed to open 'test_markdown.txt'");
	return; // this needs to be fixed and just throw;
    }
    if(!htmldown.good())
    {
	diagnostics::log_info("failed to open output 'test_htmlout.html'");
	return;
    }

    tdp::document doc;

    diagnostics::log_info("parsing filestream");
    tdp::parse(markdown, doc);
    
    diagnostics::log_info("printing document");
    tdp::print(doc.base(),htmldown);

    diagnostics::log_info("markdown_parse_test - end");
    
}

void document_structure_test()
{
    tdp::document doc;
    doc.step_in();
    tdp::node &h1 = doc.add(tdp::create_node("h1"));
    h1.content("This is a header");
    
    tdp::node &p = doc.add(tdp::create_node("p"));
    p.content("This is a paragraph");
    
    tdp::node &h2 = doc.add(tdp::create_node("h2"));
    h2.content("Another header");

    tdp::node &div = doc.add(tdp::create_node("div"));
    
    doc.step_in();
    
    tdp::node &p2 = doc.add(tdp::create_node("p"));
    p2.content("This is nested");
    std::vector<tdp::attribute> attr;
    attr.push_back(std::make_pair("src","image_src"));
    p2.content(tdp::create_content("img", attr, false));

    std::vector<tdp::attribute> href;
    href.push_back(std::make_pair("href","http://google.ca"));
    p2.content(tdp::create_content("a",href));

    doc.step_out();
    
    tdp::node &h3 = doc.add(tdp::create_node("h3"));
    h3.content("This is h3 - outside of the div");

    std::cout << "START OF TEST PRINT" << std::endl;
    tdp::print(doc.base(), std::cout);
    std::cout << "END OF TEST PRINT" << std::endl;

}


int main(int argc, const char **argv)
{

    diagnostics::enable(std::cout);

    int test(-1);
    
    std::stringstream ss;
    ss << argv[1];
    ss >> test;

    switch(test)
    {
    case all_test_val:
    case document_structure_test_val:
	document_structure_test();
	break;
    case markdown_parse_test_val:
	markdown_parse_test();
    default:
	std::cout << "unknown test -- exiting" << std::endl;
    }
}
