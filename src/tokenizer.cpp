#include "inc/tokenizer.h"

#include <cctype>


inline_header::inline_header()
	:lvl_(1)
{
}


bool inline_header::next_char(char c)
{
	if (std::isspace(c))
	{
		more(false);
		return true;
	}
	if (c == '#')
	{
		++lvl_;
	}
	else
		throw lex_error(0, 0, "bad title token");

	return false;
}

token inline_header::produce()
{
	if (needs_more())
		throw lex_error(0, 0, "unable to produce token");

	return token(token_tag::inline_header_tag); // add the level
}


ordered_list::ordered_list(char c)
{
	temp_.push_back(c);
}


bool ordered_list::next_char(char c)
{
	if (std::isdigit(c))
		temp_.push_back(c);
	else if (c == '.')
		more(false);
	else
		throw lex_error(0, 0, "bad ordered list token");
	return false;
}

token ordered_list::produce()
{
	if (needs_more())
		throw lex_error(0, 0, "unable to produce token");

	return token(ordered_list_tag); // add the number.
}


bool markdown_machine::next_char(char c)
{
	if (current_)
		return current_->next_char(c);

	if (c == '#')
	{
		current_.reset(new inline_header());
	}
	else if (std::isdigit(c))
	{
		current_.reset(new ordered_list(c));
	}
	return false;

}

bool markdown_machine::needs_more()
{
	if (current_)
		return current_->needs_more();
	
	return true;
}

token markdown_machine::produce()
{
	if (!current_)
		throw std::runtime_error("unable to produce token");
	auto discard = current_.release();
	return discard->produce();
}


tokenizer::tokenizer(std::istream &is)
	:input_(is)
{
}

token tokenizer::next()
{
	while (input_.good())
	{
		if (sm_.next_char(input_.get()))
			input_.unget();

		if (sm_.needs_more())
			return sm_.produce();
	}
	return token(eof_tag);
}