#include "inc/document.h"
#include "inc/diagnostics.h"

#include <sstream>

tdp::document::document()
{
    base_.reset(new node("html"));
    // first level 
    
    tdp::node_list html_lvl;
    html_lvl.add(base_.get());

    level_.push(html_lvl); // push on html

    tdp::node_list body_lvl;
    std::unique_ptr<node> head_ptr(new node("head"));
    base_->child(std::move(head_ptr));
    body_lvl.add(base_->child());

    std::unique_ptr<node> body_ptr(new node("body"));
    base_->child()->sibling(std::move(body_ptr));
    body_lvl.add(base_->child()->sibling()); // fkn studip;

    level_.push(body_lvl);

    child_=true;
}

tdp::node* tdp::document::base()
{
    return base_.get();
}

tdp::node& tdp::document::add(node *n)
{
    if(n==NULL)
	return *level_.top().current();

    if(child_)
    {
	// diagnostics::log_trace(id_, "adding child");
	level_.top().current()->child(std::unique_ptr<tdp::node>(n));
	tdp::node *new_top(level_.top().current()->child());

	tdp::node_list next_level;
	next_level.add(new_top);
	
	level_.push(next_level);
	child_=false;
    }
    else
    {
	// diagnostics::log_trace(id_, "adding sibling");
	level_.top().current()->sibling(std::unique_ptr<tdp::node>(n));
	level_.top().add(level_.top().current()->sibling());
    }
    return *level_.top().current();
}

void tdp::document::step_in()
{
    // diagnostics::log_trace(id_, "step in");
    child_=true;
}

void tdp::document::step_out()
{
    // diagnostics::log_trace(id_, "step out");
    child_=false;
    level_.pop();
}

tdp::node* tdp::create_node(const std::string &tag)
{
    return new node(tag);
}


std::unique_ptr<tdp::node> tdp::make_node_ptr(const std::string &tag)
{
    return std::unique_ptr<node>(new node(tag));
}

//////
// printing

void tdp::print(tdp::node *n, std::ostream &os)
{
    os << "<" << n->tag() << ">";
    if(n->child())
    {
	tdp::print(n->child(), os);
    }
    else
    {
	os << n->content();
    }
    os << "</" << n->tag() << ">";

    tdp::node *sib(n->sibling());
    
    if(sib)
	tdp::print(sib, os);
}

bool compare(const std::string &strA, const std::string &strB, int cnt)
{
    if(strA.length() < cnt || strB.length() < cnt)
	return false;
    int i(0);
    while (i < cnt)
    {
	if(strA[i] != strB[i])
	    return false;
	++i;
    }
    return true;
} 

bool is_leading_space(const std::string &str, size_t endp)
{
    size_t len(endp);
    if(endp > str.size() )
	len=str.size();

    int pos(0);
    while(pos<len)
    {
	if(!std::isspace(str.at(pos)) )
	   return false;

	++pos;
    }
    return true;;
}

void tdp::parse(std::istream &is, tdp::document &doc)
{
    diagnostics::log_info("entering parse");
    int lines_parsed(0);
    std::string line;
    document_state_machine dsm(doc);

    diagnostics::log_info("register first state");
    dsm.register_transition<tdp::wait_for_line_state, tdp::read_block, tdp::wait_for_block_end_state>();
    
    diagnostics::log_info("register second state");
    dsm.register_transition<tdp::wait_for_block_end_state, tdp::wait_for_line, tdp::wait_for_line_state>();

    diagnostics::log_info("register third state");
    dsm.register_transition<tdp::wait_for_line_state, tdp::read_unordered_list, tdp::read_unordered_list_state>();
    

    dsm.register_transition<tdp::read_unordered_list_state, tdp::wait_for_line, tdp::wait_for_line_state>();

    diagnostics::log_info("dsm begin");
    dsm.begin(tdp::wait_for_line_state::id);
    
    while(is.good())
    {
	std::stringstream ss;
	ss << "iterations: "<< ++lines_parsed;
	diagnostics::log_info(ss.str());

	ss.str("");
	getline(is, line); // read a line

	ss << "line read: " << line;
	diagnostics::log_info(ss.str());

	// # H1, ## H2, ### H3, #### H4, ##### H5, ###### H6
	// Title
	// ===
	// Heading 2
	// ---

	// - 
	dsm.parse_line(line);
	
	
	diagnostics::log_info("");
	

    }


}

using namespace tdp;
/////
// state_machine
document_state_machine::document_state_machine(document &doc)
    :doc_(doc)
{
}

void document_state_machine::parse_line(const std::string &line)
{
    last_line_=line;
    document_state* cur_state = dynamic_cast<document_state*>(current());
    cur_state->parse_line(line);
}

const std::string& document_state_machine::get_last_line()
{
    return last_line_;
}

/////
// wait_for_line_state
const int wait_for_line_state::id = 1;

void wait_for_line_state::on_enter_state()
{
    diagnostics::log_info("wait_for_line::on_enter_state()");
}

void wait_for_line_state::on_exit_state(int reason)
{
    diagnostics::log_info("wait_for_line::on_exit_state");
}

void wait_for_line_state::parse_line(const std::string &line)
{
	if( line.empty() )
	{
	    diagnostics::log_info("empty line");
	    return;
	}
		
	if( compare(line, "# ", 2) )
	{
	    diagnostics::log_info("adding heading 1");
	    tdp::node &n = get_state_machine()->get_document().add(tdp::create_node("h1"));
	    n.content(line.substr(2));
	}
	else if( compare(line, "## ", 3) )
	{
	    diagnostics::log_info("adding heading 2");
	    tdp::node &n = get_state_machine()->get_document().add(tdp::create_node("h2"));
	    n.content(line.substr(3));
	}
	else if( compare(line, "### ", 4) )
	{
	    diagnostics::log_info("adding heading 3");
	    tdp::node &n = get_state_machine()->get_document().add(tdp::create_node("h3"));
	    n.content(line.substr(4));
	}
	else if( compare(line, "#### ", 5) )
	{
	    diagnostics::log_info("adding heading 4");
	    tdp::node &n = get_state_machine()->get_document().add(tdp::create_node("h4"));
	    n.content(line.substr(5));
	}
	else if( compare(line, "##### ", 6) )
	{
	    diagnostics::log_info("adding heading 5");
	    tdp::node &n = get_state_machine()->get_document().add(tdp::create_node("h5"));
	    n.content(line.substr(6));
	}
	else if( compare(line, "###### ", 7) )
	{
	    diagnostics::log_info("adding heading 6");
	    tdp::node &n = get_state_machine()->get_document().add(tdp::create_node("h6"));
	    n.content(line.substr(7));
	}
	else if( line.find_first_of("+-") != std::string::npos )
	{
	    if(is_leading_space(line,line.find_first_of("+-"))) // inefficient IU knwo
	    {
		diagnostics::log_info("starting unordered list");
		end_state(read_unordered_list);
	    }
	}
	else
	{
	    end_state(read_block);
	}

}

int wait_for_line_state::state_id()
{
    return wait_for_line_state::id;
}

const int wait_for_block_end_state::id = 2;

void wait_for_block_end_state::on_enter_state()
{
    diagnostics::log_info("wait_for_block_end::on_enter_state()");
    block_ << get_state_machine()->get_last_line();
}

void wait_for_block_end_state::on_exit_state(int reason)
{
    diagnostics::log_info("wait_for_block_end::on_exit_state()");
}

void wait_for_block_end_state::parse_line(const std::string &line)
{
	if( compare(line, "===", 3) )
	{
	    diagnostics::log_info("adding alt heading 1");
	    tdp::node &n = get_state_machine()->get_document().add(tdp::create_node("h1"));
	    n.content(block_.str());
	    block_.str("");
	    end_state(wait_for_line);
	}
	else if( compare(line, "---", 3) )
	{
	    diagnostics::log_info("adding alt heading 2");
	    tdp::node &n = get_state_machine()->get_document().add(tdp::create_node("h2"));
	    n.content(block_.str());
	    block_.str("");
	    end_state(wait_for_line);
	}
	else if( line.empty() )
	{
	    diagnostics::log_info("adding paragraph");
	    tdp::node &n = get_state_machine()->get_document().add(tdp::create_node("p"));
	    n.content(block_.str());
	    block_.str("");
	    end_state(wait_for_line);
	    // append to paragraph
	}
	else
	    block_<<line;


}

int wait_for_block_end_state::state_id()
{
    return wait_for_block_end_state::id;
}

const int read_unordered_list_state::id = 3;

void read_unordered_list_state::on_enter_state()
{
    diagnostics::log_info("read_unordered_list_state::on_enter_state");
    tdp::document &doc=get_state_machine()->get_document();
    doc.add(tdp::create_node("ul"));
    doc.step_in();
    tdp::node &it=doc.add(tdp::create_node("li"));
    std::string last(get_state_machine()->get_last_line());
    tpos_=(last.find_first_of("+-"))%3; //I want tab of 3
    it.content(last.substr(tpos_+1));
}

void read_unordered_list_state::on_exit_state(int reason)
{
    diagnostics::log_info("read_unordered_list_state::on_exit_state");
    get_state_machine()->get_document().step_out();
}

void read_unordered_list_state::parse_line(const std::string &line)
{
    if( line.empty() )
    {
	diagnostics::log_info("parse_line empty read ending");
	end_state(wait_for_line);
	return;
    }
    
    tdp::document &doc = get_state_machine()->get_document();

    size_t pos=line.find_first_of("+-");
 
    bool iswhite=is_leading_space(line,pos);

    if( tpos_>3 && pos < (tpos_-3) )
    {
	diagnostics::log_info("list parse step out");
	doc.step_out();
    }
    else if( pos >= (tpos_+3) )
    {						
	diagnostics::log_info("list parse step in");
	doc.add(tdp::create_node("ul"));
	doc.step_in();
	tpos_+=3;
    }
    
    if(pos == std::string::npos || !is_leading_space(line, pos))
    {
	// add it to the current
    }
    else
    {
	diagnostics::log_info("list parse add item");
	tdp::node &n=doc.add(tdp::create_node("li"));
	n.content(line.substr(pos+1));
    }

}

int read_unordered_list_state::state_id()
{
    return read_unordered_list_state::id;
}



