#ifndef __DOCUMENT_H__
#define __DOCUMENT_H__

#include <memory>
#include <iostream>
#include <stack>
#include <vector>
#include <sstream>

// for std::unique_ptr because it requires the class destructor
#include "inc/node.h"
#include "inc/state_machine.h"

namespace tdp
{
class node_list
{
    std::vector<node *> list_;
public:
    node* current() { return list_.at(list_.size()-1); }
    node* add(node *n) { list_.push_back(n); return current(); }
};

class document
{
    std::unique_ptr<node> base_;

    std::stack<node_list> level_;
    node* sibling_;

    bool child_;
public:
    document();
    node* base();

    
    node& current();
    node& add(node *n);
    void step_in();
    void step_out();

};

node* create_node(const std::string &tag);
std::unique_ptr<node> make_node_ptr(const std::string &tag);

void print(node *n, std::ostream &os);

void parse(std::istream &is, document &doc);

class document_state_machine : public finite::state_machine
{
    document &doc_;
    std::string last_line_;
public:
    document& get_document() { return doc_; }
    document_state_machine(document &doc);
    void parse_line(std::string &line);
    const std::string & get_last_line();

};

class document_state : public finite::state
{
protected:
    document_state_machine* get_state_machine()
    {
	return dynamic_cast<document_state_machine*>(finite::state::get_state_machine());
    }
public:
    
    virtual void parse_line(std::string &line)=0;
};

const int wait_for_line=0x01;
const int read_block=0x02;
const int read_unordered_list=0x03;

class wait_for_line_state : public document_state
{

public:
    static const int id;
    virtual void on_enter_state();
    virtual void on_exit_state(int reason);
    virtual void parse_line(std::string &line) override;
    virtual int state_id();
};

class wait_for_block_end_state : public document_state
{
    std::stringstream block_;
public:
    static const int id;
    virtual void on_enter_state();
    virtual void on_exit_state(int reason);
    virtual void parse_line(std::string &line) override;
    virtual int state_id();
};

class read_unordered_list_state : public document_state
{
    size_t tpos_; //tabbed position
public:
    static const int id;
    virtual void on_enter_state();
    virtual void on_exit_state(int reason); 
    virtual void parse_line(std::string &line) override;
    virtual int state_id();
};

};




#endif // __DOCUMENT_H__
