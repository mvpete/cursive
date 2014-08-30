#include "inc/node.h"

#include <sstream>

using namespace tdp;


string_content::string_content(const std::string & text)
    :inner_text_(text)
{
}

std::string string_content::to_string() const
{
    return inner_text_;
}


node_content::node_content(const std::string &tag, const std::vector<attribute> &attr, bool bookend /*=true*/)
    :tag_(tag),attr_(attr),bookend_(bookend)
{
}

void node_content::inner(const std::string &str)
{
    inner_text_=str;
}

std::string node_content::to_string() const
{
    std::stringstream ss;
    ss << "<" << tag_ << " ";
    for(attribute attr : attr_)
    {
	ss<< attr.first << "='" << attr.second <<"' ";
    }
    if(!bookend_)
    {
	ss << "/>";
    }
    else
    {
	ss << ">" << inner_text_ <<  "</" << tag_ << ">";
    }

    return ss.str();
}

std::unique_ptr<icontent> tdp::create_content(const std::string &tag, const std::vector<attribute> &attr, bool bookend)
{
    return std::unique_ptr<icontent>(new node_content(tag,attr,bookend));
}

///////
// node

tdp::node::node()
    :tag_("")
{
}

tdp::node::node(const std::string &tag)
    :tag_(tag),rebuild_content_(true)
{
}

tdp::node::~node()
{
}

tdp::node* tdp::node::sibling(std::unique_ptr<node> sibling)
{
    sibling_.reset(sibling.release());
    return sibling_.get();
}

tdp::node* tdp::node::child(std::unique_ptr<node> child)
{
    child_.reset(child.release());
    return child_.get();
}
 
tdp::node* tdp::node::sibling()
{
    return sibling_.get();
}

tdp::node* tdp::node::child()
{
    return child_.get();
}  

const std::string& tdp::node::tag()
{
    return tag_;
}

void tdp::node::content(const std::string &cstr)
{
    flag_rebuild();
    contents_.push_back(std::unique_ptr<tdp::icontent>(new string_content(cstr)));
}

void tdp::node::content(std::unique_ptr<icontent> new_content)
{
    flag_rebuild();
    contents_.push_back(std::move(new_content));
}

const std::string& tdp::node::content()
{
    if(rebuild_content_)
    {
	std::stringstream ss;
	for(std::unique_ptr<tdp::icontent> &article : contents_)
	{
	    ss << article->to_string();
	}
	content_str_=ss.str();
    }

    return content_str_;
}

void tdp::node::flag_rebuild(bool flag/* =true*/)
{
    rebuild_content_=flag;
}
