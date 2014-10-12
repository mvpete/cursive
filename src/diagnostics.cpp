#include "inc/diagnostics.h"

std::unique_ptr<diagnostics> diagnostics::instance_;

void diagnostics::enable(std::ostream &os, diagnostics::levels lvl)
{
	if (!instance_)
	{
		instance_.reset(new diagnostics());
		instance_->set_ostream(&os);
		instance_->set_level(lvl);
	}
}


void diagnostics::log_info(const std::string &str)
{
	if (!instance_)
		return;
	instance_->log(diagnostics::info, str);
}


void diagnostics::log_trace(const std::string &str)
{
	if (!instance_)
		return;
	instance_->log(diagnostics::trace, str);
}

void diagnostics::log_warn(const std::string &str)
{
	if (!instance_)
		return;
	instance_->log(diagnostics::warn, str);
}

void diagnostics::log_err(const std::string &str)
{
	if (!instance_)
		return;
	instance_->log(diagnostics::err, str);
}


void diagnostics::set_ostream(std::ostream *os)
{
	osp_ = os;
}

void diagnostics::set_level(diagnostics::levels plvl)
{
	plevel_ = plvl;
}

void diagnostics::log(levels lvl, const std::string &msg)
{
	if (plevel_ < lvl)
		return;


	switch (lvl)
	{
	case diagnostics::trace:
		*osp_ << "TRACE: ";
	case diagnostics::info:
		*osp_ << "INFO: ";
		break;
	case diagnostics::warn:
		*osp_ << "WARN: ";
		break;
	case diagnostics::err:
		*osp_ << "ERR: ";
		break;
	}
	*osp_ << msg.c_str() << std::endl;
}



