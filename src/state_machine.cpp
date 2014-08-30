#include "inc/state_machine.h"


using namespace finite;

////// 
/// state - 
void state::register_machine(state_machine *machine)
{
    machine_=machine;
}

void state::end_state(int tcode)
{
    if(!machine_)
    {
	// diagnostics::log_error(id_, "no state machine registered");
	return;
    }
    machine_->end_state(tcode);
}


/////////
// state_machine

state* state_machine::current()
{
    return current_;
}

state* state_machine::find_state_by_id(int state_id)
{
    
    auto i=states_.begin(),i_end=states_.end();
    for(; i!=i_end; ++i)
    {
	if((*i)->state_id() == state_id)
	    return i->get();
    }
    return NULL;
}

void state_machine::begin(int state_id)
{
    current_=find_state_by_id(state_id);
    if(!current_)
	throw std::logic_error("failed to begin - state not found");
}

void state_machine::change_state(int state_id, int tcode)
{
    state *state = find_state_by_id(state_id);
    if(!state)
	throw std::logic_error("failed to find next state");
    current_->on_exit_state(tcode);
    current_=state;
    current_->on_enter_state();
}

void state_machine::end_state(int tcode)
{
    auto i(transitions_.begin()), i_end(transitions_.end());
    for(; i!=i_end; ++i)
    {
	if(i->s1 == current_->state_id() && i->tcode == tcode)
	{
	    change_state(i->s2, tcode);
	    return;
	}
    }
    throw std::logic_error("no transition found");
}
