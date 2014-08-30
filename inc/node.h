#ifndef __NODE_H__
#define __NODE_H__

#include <memory>
#include <string>
#include <vector>


// project shorthand

namespace tdp
{
    typedef std::pair<std::string,std::string> attribute;

    class icontent
    {
    public:
	virtual ~icontent(){}
	virtual std::string  to_string() const=0;
    };

    class string_content : public icontent
    {
        std::string inner_text_;
    public:
	string_content(const std::string &text);
	virtual std::string to_string() const override;
    };

    class node_content : public icontent
    {
	// this should have a node so it can just nest content
	std::string tag_;
	std::vector<attribute> attr_;
	std::string inner_text_;
	bool bookend_;
	node_content(node_content&)=delete;


    public:
	node_content(const std::string &tag, const std::vector<attribute> &attr, bool bookend = true);
	void inner(const std::string &str);
	virtual std::string to_string() const override;
	
    };
    

    class node
    {
    public:
	typedef std::pair<std::string,std::string> attribute;
	typedef std::vector<attribute> attribute_list;

	class node_cc
	{
	};

    private:
	bool rebuild_content_;
	void flag_rebuild(bool rebuild=true);
	std::string content_str_;

    protected:
	std::vector<std::unique_ptr<icontent>> contents_;
	std::string tag_;
	attribute_list  attributes_;
	bool bookend_;
	
    private:
	std::unique_ptr<node> sibling_;
	std::unique_ptr<node> child_;
    public:
	node(const std::string &string);
	node();
	virtual ~node();
	tdp::node* sibling(std::unique_ptr<node> sibling);
	tdp::node* child(std::unique_ptr<node>   child);
	node* sibling();
	node* child();

	virtual const std::string& tag();
	void content(const std::string &content);
	void content(std::unique_ptr<icontent> new_content);
	virtual const std::string& content();
    };

    std::unique_ptr<icontent> create_content(const std::string &tag, const std::vector<attribute> &attr, bool bookend=true);

};


#endif // __NODE_H__
