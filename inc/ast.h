#ifndef __AST_H__
#define __AST_H__

#include <string>
#include <vector>

#include <boost/variant.hpp>

// Just a type alias for a variant. Serves as a general purpose node
using markdown_ast = boost::variant<std::string,
									struct markdown_image,
									struct markdown_image_unresolved,
									boost::recursive_wrapper<struct markdown_group>,
									boost::recursive_wrapper<struct markdown_header>,
									boost::recursive_wrapper<struct markdown_paragraph>,
									boost::recursive_wrapper<struct markdown_blockquote>,
									boost::recursive_wrapper<struct markdown_code>,
									boost::recursive_wrapper<struct markdown_inline_code>,
									boost::recursive_wrapper<struct markdown_link>,
									boost::recursive_wrapper<struct markdown_unresolved_link>,
									boost::recursive_wrapper<struct markdown_emphasis>,
									boost::recursive_wrapper<struct markdown_strong_emphasis>,
									boost::recursive_wrapper<struct markdown_strikethrough>
									>;

// A group of any type of markdown elements								
struct markdown_group
{
	std::vector<markdown_ast> contents;
}

struct markdown_header
{
	markdown_ast contents;
	unsigned int level;
}

struct markdown_paragraph
{
	markdown_ast contents;
}

struct markdown_blockquote
{
	markdown_ast contents;
}

// A preformatted code block; has it's own std::string rather than using
// to markdown_ast since it can ONLY hold preformatted text
struct markdown_code
{
	std::string text;
}

// Same as above but inline with adjacent text
struct markdown_inline_code
{
	std::string text;
}

// Hyperlink
struct markdown_link
{
	markdown_ast contents;
	std::string url;
}

// Unresolved hyperlink, for documents that put the URLs at the end
struct markdown_unresolved_link
{
	markdown_ast contents;
	std::string ref_text;
}

struct markdown_image
{
	std::string alt_text;
	std::string url;
}

struct markdown_image_unresolved
{
	std::string alt_text;
	std::string ref_text;
}

// Italics
struct markdown_emphasis
{
	markdown_ast contents;
}

// Bold
struct markdown_strong_emphasis
{
	markdown_ast contents;
}

struct markdown_strikethrough
{
	markdown_ast contents;
}

struct 
									
#endif // __AST_H__