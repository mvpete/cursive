#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include "ast.h"
#include "state_machine.h"


enum token_tag
{
	char_string_tag,
	inline_header_tag,
	ordered_list_tag,
	eof_tag = EOF,
};

class token
{
public:
	token(token_tag t); // forn ow
};

class state
{
	bool needs_more_;
protected:
	
	void more(bool nm) { needs_more_ = nm; }
public:
	state() :needs_more_(true){}
	virtual ~state();
	bool needs_more() { return needs_more_; }
	virtual bool next_char(char c); // remember that the return on next_char is used to push the char back
	virtual token produce();
};

class inline_header : public state
{
	int lvl_;
public:
	inline_header();
	virtual bool next_char(char c);
	virtual token produce();
};

class ordered_list : public state
{
	std::string temp_;
public:
	ordered_list(char c);
	virtual bool next_char(char c);
	virtual token produce();
};

class markdown_machine 
{
	std::unique_ptr<state> current_;

public:
	
	bool next_char(char c); // return true to replace current token
	bool needs_more();
	token produce();

};




class lex_error : public std::exception
{
	int line_, col_;
public:
	lex_error(int line, int col, const char *msg):std::exception(msg),line_(line),col_(col){};

};





class tokenizer
{
	markdown_machine sm_;
	std::istream &input_;

public:
	tokenizer(std::istream &input);
	token next();
};





#endif //__TOKENIZER_H__