#ifndef __DIAGNOSTICS_H__
#define __DIAGNOSTICS_H__
#include <iostream>
#include <memory>

class diagnostics
{
public:
	enum levels { trace=3, info=2, warn=1, err=0 };
private:
	levels plevel_;
    std::ostream *osp_;

    void set_ostream(std::ostream *os);
	void set_level(levels plvl);

    static std::unique_ptr<diagnostics> instance_;
    void log(levels lvl, const std::string &msg);
    
public:
    static void enable(std::ostream &os, levels print_level = levels::warn);
    static void log_info(const std::string &str);
	static void log_trace(const std::string &str);
	static void log_warn(const std::string &str);
	static void log_err(const std::string &str);
};

#endif // __DIAGNOSTICS_H__
