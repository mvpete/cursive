#include "inc/diagnostics.h"

std::unique_ptr<diagnostics> diagnostics::instance_;

void diagnostics::enable(std::ostream &os)
{
	if (!instance_)
	{
		instance_.reset(new diagnostics());
		instance_->set_ostream(&os);
	}
}


void diagnostics::log_info(const std::string &str)
{
	if (!instance_)
		return;
	instance_->log(diagnostics::info, str);
}


void diagnostics::set_ostream(std::ostream *os)
{
	osp_ = os;
}

void diagnostics::log(levels lvl, const std::string &msg)
{
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



