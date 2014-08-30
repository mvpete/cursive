#ifndef __NODE_SM_H__
#define __NODE_SM_H__

#include <string>
#include <memory>
#include <vector>

namespace finite
{
    class state_machine;
      
    class state
    {
	state_machine *machine_;
    protected:
	state_machine *get_state_machine() { return machine_; }
    public:
	virtual ~state(){};
	void register_machine(state_machine *machine);
	void end_state(int tcode);
	virtual void on_enter_state(){};
	virtual void on_exit_state(int tcode){};
	virtual int state_id()=0;
    };
   
    struct transition
    {
	transition(const int s1, int tcode, const int s2)
	:s1(s1),tcode(tcode),s2(s2){}
	const int s1, s2;
	int tcode;
    };
    
    class state_machine
    {
    
	state *current_;
	std::vector<std::unique_ptr<state> > states_;
	std::vector<transition> transitions_;
	void change_state(int state_id, int tcode);
    protected:
	
	state* current();
	state* find_state_by_id(int state_id);
    public:
	virtual ~state_machine(){};
	void begin(int state_id);
	void end_state(int tcode);
	void parse_line(const std::string &line);
	
	template<class T>
	class owned_ptr
	{
	    T *ptr_;
	    bool owner_;

	public:
	    owned_ptr(T *ptr, bool owner=true)
		:ptr_(ptr),owner_(owner)
	    {
	    }
	    ~owned_ptr()
	    {
		if(owner_)
		    delete ptr_;
	    }
	    owned_ptr(const owned_ptr &rhs)
		:ptr_(rhs.get()),owner_(false)
	    {
	    }
	    bool is_owner()
	    {
		return owner_;
	    }
	    void reset(T* ptr, bool owner=true)
	    {
		if(owner_)
		    delete ptr_;
		ptr_=ptr;
		owner_=owner;
	    }
	    const T* get() const
	    {
		return ptr_;
	    }

	    T* get()
	    {
		return ptr_;
	    }

	    T* release()
	    {
		T* tmp(ptr_);
		ptr_=0;
		return tmp;
	    }
	};

	template <typename S1,int reason,typename S2>
	void register_transition()
	{
	   

	    owned_ptr<state> s1(find_state_by_id(S1::id),false);
	    owned_ptr<state> s2(find_state_by_id(S2::id),false);
	  
	    if(!s1.get())
	    {
		s1.reset(new S1());
	    }
	    if(!s2.get())
	    {
		s2.reset(new S2());
	    }

	    auto i=transitions_.begin(),i_end=transitions_.end();
	    for(; i!=i_end; ++i)
	    {
		if(i->s1 == s1.get()->state_id() && reason==i->tcode)
		    throw std::logic_error("transition duplicated");
	    }
	    
	    transitions_.push_back(transition(s1.get()->state_id(),reason,s2.get()->state_id()));
	    
	    if(s1.is_owner())
	    {
		s1.get()->register_machine(this);
		states_.push_back(std::unique_ptr<state>(s1.release()));
	    }
	    if(s2.is_owner())
	    {
		s2.get()->register_machine(this);
		states_.push_back(std::unique_ptr<state>(s2.release()));
	    }

	}


    };
};


#endif //__NODE_SM_H__
